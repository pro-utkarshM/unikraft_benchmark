/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (c) 2023, Unikraft GmbH and The Unikraft Authors.
 * Licensed under the BSD-3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 */

#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <uk/alloc.h>
#include <uk/assert.h>
#include <uk/config.h>
#include <uk/init.h>
#include <uk/syscall.h>

#include <uk/posix-fdtab.h>

#include "fmap.h"

#if CONFIG_LIBPOSIX_FDTAB_MULTITAB
#include <uk/essentials.h>
#include <uk/refcount.h>
#include <uk/thread.h>
#endif /* CONFIG_LIBPOSIX_FDTAB_MULTITAB */

#if CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM
#include <uk/posix-fdtab-legacy.h>
#endif /* CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM */

#if CONFIG_LIBPOSIX_PROCESS_CLONE
#include <uk/process.h>
#endif /* CONFIG_LIBPOSIX_PROCESS_CLONE */

#if CONFIG_LIBPOSIX_PROCESS_EXECVE
#include <uk/event.h>
#include <uk/prio.h>
#endif /* CONFIG_LIBPOSIX_PROCESS_EXECVE */

#define UK_FDTAB_SIZE CONFIG_LIBPOSIX_FDTAB_MAXFDS
UK_CTASSERT(UK_FDTAB_SIZE <= UK_FD_MAX);


struct uk_fdtab {
	struct uk_alloc *alloc;
	struct uk_fmap fmap;
#if CONFIG_LIBPOSIX_FDTAB_MULTITAB
	__atomic refcnt;
#endif /* CONFIG_LIBPOSIX_FDTAB_MULTITAB */
	unsigned long _bmap[UK_BMAP_NELEM(UK_FDTAB_SIZE)];
	void *_fdmap[UK_FDTAB_SIZE];
};

static struct uk_fdtab init_fdtab = {
	.fmap = {
		.bmap = {
			.size = UK_FDTAB_SIZE,
			.bitmap = init_fdtab._bmap
		},
		.map = init_fdtab._fdmap
	},
#if CONFIG_LIBPOSIX_FDTAB_MULTITAB
	.refcnt = UK_REFCOUNT_INITIALIZER(1)
#endif /* CONFIG_LIBPOSIX_FDTAB_MULTITAB */
};

#if CONFIG_LIBPOSIX_FDTAB_MULTITAB

/* Every thread keeps its own fdtab reference */
static __uk_tls struct uk_fdtab *active_fdtab;

#else /* !CONFIG_LIBPOSIX_FDTAB_MULTITAB */

/* All threads share the same static init fdtab */
static struct uk_fdtab *const active_fdtab = &init_fdtab;

#endif /* !CONFIG_LIBPOSIX_FDTAB_MULTITAB */

static int init_posix_fdtab(struct uk_init_ctx *ictx __unused)
{
	init_fdtab.alloc = uk_alloc_get_default();
	/* Consider skipping init for .map (static vars are inited to 0) */
	uk_fmap_init(&init_fdtab.fmap);
#if CONFIG_LIBPOSIX_FDTAB_MULTITAB
	/* Ensure the init thread has a valid fdtab ref */
	uk_refcount_acquire(&init_fdtab.refcnt);
	active_fdtab = &init_fdtab;
#endif /* CONFIG_LIBPOSIX_FDTAB_MULTITAB */
	return 0;
}

/* Encode flags in entry pointer using the least significant bits */
/* made available by the open file structure's alignment */
struct fdval {
	void *p;
	int flags;
};

#define UK_FDTAB_CLOEXEC 1

#if CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM
#define UK_FDTAB_VFSCORE 2
#define _MAX_FLAG 2
#else /* !CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM */
#define _MAX_FLAG 1
#endif /* !CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM */

#define _FLAG_MASK (((uintptr_t)_MAX_FLAG << 1) - 1)

UK_CTASSERT(__alignof__(struct uk_ofile) > _MAX_FLAG);
#if CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM && CONFIG_LIBVFSCORE
UK_CTASSERT(__alignof__(struct vfscore_file) > _MAX_FLAG);
#endif /* CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM && CONFIG_LIBVFSCORE */

static inline const void *fdtab_encode(const void *f, int flags)
{
	UK_ASSERT(!((uintptr_t)f & _FLAG_MASK));
	return (const void *)((uintptr_t)f | flags);
}

static inline struct fdval fdtab_decode(void *p)
{
	uintptr_t v = (uintptr_t)p;

	return (struct fdval) {
		.p = (void *)(v & ~_FLAG_MASK),
		.flags = v & _FLAG_MASK
	};
}

static inline void file_acq(void *p, int flags __maybe_unused)
{
#if CONFIG_LIBVFSCORE
	if (flags & UK_FDTAB_VFSCORE)
		fhold((struct vfscore_file *)p);
	else
#endif /* CONFIG_LIBVFSCORE */
		uk_ofile_acquire((struct uk_ofile *)p);
}

static inline void file_rel(void *p, int flags __maybe_unused)
{
#if CONFIG_LIBVFSCORE
	if (flags & UK_FDTAB_VFSCORE)
		fdrop((struct vfscore_file *)p);
	else
#endif /* CONFIG_LIBVFSCORE */
		uk_ofile_release((struct uk_ofile *)p);
}

/* Ops */

int uk_fdtab_open_desc(struct uk_ofile *of, unsigned int mode)
{
	int fd;
	const void *entry = fdtab_encode(of,
		(mode & O_CLOEXEC) ? UK_FDTAB_CLOEXEC : 0);

	fd = uk_fmap_put(&active_fdtab->fmap, entry, 0);
	if (unlikely(fd >= UK_FDTAB_SIZE))
		return -ENFILE;
	return fd;
}

int uk_fdtab_open_named(const struct uk_file *f, unsigned int mode,
			const char *name, size_t len)
{
	struct uk_ofile *of;
	int fd;

	UK_ASSERT(f);
	if (!name)
		len = 0;

	of = uk_ofile_new(f, mode, len);
	if (unlikely(!of))
		return -ENOMEM;
	if (len) {
		memcpy(of->name, name, len);
		of->name[len] = 0;
	}

	/* Place the file in fdtab */
	fd = uk_fdtab_open_desc(of, mode);
	if (unlikely(fd < 0))
		uk_ofile_release(of);
	return fd;
}

int uk_fdtab_open(const struct uk_file *f, unsigned int mode)
{
	return uk_fdtab_open_named(f, mode, NULL, 0);
}

int uk_fdtab_setflags(int fd, int flags)
{
	struct uk_fmap *fmap;
	void *p;
	struct fdval v;
	const void *newp;

	if (flags & ~O_CLOEXEC)
		return -EINVAL;

	fmap = &active_fdtab->fmap;

	p = uk_fmap_critical_take(fmap, fd);
	if (!p)
		return -EBADF;
	v = fdtab_decode(p);
	v.flags &= ~UK_FDTAB_CLOEXEC;
	v.flags |= flags ? UK_FDTAB_CLOEXEC : 0;

	newp = fdtab_encode(v.p, v.flags);
	uk_fmap_critical_put(fmap, fd, newp);
	return 0;
}

int uk_fdtab_getflags(int fd)
{
	void *p = uk_fmap_lookup(&active_fdtab->fmap, fd);
	struct fdval v;
	int ret;

	if (!p)
		return -EBADF;

	v = fdtab_decode(p);
	ret = 0;
	if (v.flags & UK_FDTAB_CLOEXEC)
		ret |= O_CLOEXEC;
	return ret;
}

#if CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM
#if CONFIG_LIBVFSCORE
int uk_fdtab_legacy_open(struct vfscore_file *vf)
{
	const void *entry;
	int fd;

	fhold(vf);
	entry = fdtab_encode(vf, UK_FDTAB_VFSCORE);
	fd = uk_fmap_put(&active_fdtab->fmap, entry, 0);
	if (fd >= UK_FDTAB_SIZE)
		goto err_out;
	vf->fd = fd;
	return fd;
err_out:
	fdrop(vf);
	return -ENFILE;
}

struct vfscore_file *uk_fdtab_legacy_get(int fd)
{
	struct uk_fmap *fmap = &active_fdtab->fmap;
	struct vfscore_file *vf = NULL;
	void *p = uk_fmap_critical_take(fmap, fd);

	if (p) {
		struct fdval v = fdtab_decode(p);

		if (v.flags & UK_FDTAB_VFSCORE) {
			vf = (struct vfscore_file *)v.p;
			fhold(vf);
		}
		uk_fmap_critical_put(fmap, fd, p);
	}
	return vf;
}
#endif /* CONFIG_LIBVFSCORE */

int uk_fdtab_shim_get(int fd, union uk_shim_file *out)
{
	struct uk_fmap *fmap;
	void *p;

	if (fd < 0)
		return -1;

	fmap = &active_fdtab->fmap;

	p = uk_fmap_critical_take(fmap, fd);
	if (p) {
		struct fdval v = fdtab_decode(p);

#if CONFIG_LIBVFSCORE
		if (v.flags & UK_FDTAB_VFSCORE) {
			struct vfscore_file *vf = (struct vfscore_file *)v.p;

			fhold(vf);
			uk_fmap_critical_put(fmap, fd, p);
			out->vfile = vf;
			return UK_SHIM_LEGACY;
		} else
#endif /* CONFIG_LIBVFSCORE */
		{
			struct uk_ofile *of = (struct uk_ofile *)v.p;

			uk_ofile_acquire(of);
			uk_fmap_critical_put(fmap, fd, p);
			out->ofile = of;
			return UK_SHIM_OFILE;
		}
	}
	return -1;
}
#endif /* CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM */

static struct fdval _fdtab_get(struct uk_fdtab *tab, int fd)
{
	struct fdval ret = { NULL, 0 };

	if (fd >= 0) {
		/* Need to refcount atomically => critical take & put */
		struct uk_fmap *fmap = &tab->fmap;
		void *p = uk_fmap_critical_take(fmap, fd);

		if (p) {
			ret = fdtab_decode(p);
			file_acq(ret.p, ret.flags);
			uk_fmap_critical_put(fmap, fd, p);
		}
	}
	return ret;
}

struct uk_ofile *uk_fdtab_get(int fd)
{
	struct fdval v = _fdtab_get(active_fdtab, fd);

#if CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM
	/* Report legacy files as not present if called through new API */
	if (v.p && v.flags & UK_FDTAB_VFSCORE) {
		file_rel(v.p, v.flags);
		return NULL;
	}
#endif /* CONFIG_LIBPOSIX_FDTAB_LEGACY_SHIM */
	return (struct uk_ofile *)v.p;
}

static void fdtab_cleanup(struct uk_fdtab *tab, int all)
{
	struct uk_fmap *fmap = &tab->fmap;

	for (int i = 0; i < UK_FDTAB_SIZE; i++) {
		void *p = uk_fmap_lookup(fmap, i);

		if (p) {
			struct fdval v = fdtab_decode(p);

			if (all || (v.flags & UK_FDTAB_CLOEXEC)) {
				void **pp __maybe_unused;

				pp = uk_fmap_take(fmap, i);
				UK_ASSERT(p == pp);
				file_rel(v.p, v.flags);
			}
		}
	}
}

void uk_fdtab_cloexec(void)
{
	fdtab_cleanup(active_fdtab, 0);
}

#if CONFIG_LIBPOSIX_PROCESS_EXECVE
static int fdtab_handle_execve(void *data __unused)
{
	uk_fdtab_cloexec();
	return UK_EVENT_HANDLED_CONT;
}

UK_EVENT_HANDLER_PRIO(POSIX_PROCESS_EXECVE_EVENT, fdtab_handle_execve,
		      UK_PRIO_EARLIEST);
#endif /* CONFIG_LIBPOSIX_PROCESS_EXECVE */

/* Cleanup all leftover open fds in the initial fdtab */
static void term_posix_fdtab(const struct uk_term_ctx *tctx __unused)
{
	fdtab_cleanup(&init_fdtab, 1);
}

#if CONFIG_LIBPOSIX_FDTAB_MULTITAB

/* When using multi-fdtabs, the init thread has a ref to the init fdtab.
 *
 * Newly-created raw threads will start off with a copy of their parent's ref,
 * as a compatibility stop-gap.
 * It is the responsibility of other posix libs and their callbacks to init
 * a new thread's fdtab ref, unsharing as necessary (e.g., clone handler either
 * copying a ref or duplicating the entire fdtab).
 */

/**
 * Duplicate the current fdtab and return a pointer to the copy.
 *
 * NOTE: This is a basic implementation that does not guarantee atomicity of the
 * clone operation; we optimistically assume this will not break anything.
 * Please revisit if this turns out to be false.
 *
 * @param tab fdtab to duplicate
 *
 * @return
 *  != NULL: Success, pointer to copy
 *  == NULL: Failed to allocate memory
 */
static struct uk_fdtab *fdtab_duplicate(struct uk_fdtab *tab)
{
	struct uk_fdtab *ret = uk_malloc(tab->alloc, sizeof(*ret));

	if (unlikely(!ret))
		return NULL;

	ret->alloc = tab->alloc;
	ret->fmap = (struct uk_fmap){
		.bmap = {
			.size = UK_FDTAB_SIZE,
			.bitmap = (unsigned long *)ret->_bmap
		},
		.map = ret->_fdmap
	};
	uk_refcount_init(&ret->refcnt, 1);
	for (int i = 0; i < UK_FDTAB_SIZE; i++) {
		struct fdval v = _fdtab_get(tab, i);
		const void *entry = NULL;

		if (v.p)
			entry = fdtab_encode(v.p, v.flags);
		uk_fmap_set(&ret->fmap, i, entry);
	}
	return ret;
}

static void fdtab_free(struct uk_fdtab *tab)
{
	fdtab_cleanup(tab, 1);
	uk_free(tab->alloc, tab);
}

static int fdtab_thread_init(struct uk_thread *child,
			     struct uk_thread *parent)
{
	struct uk_fdtab *tab;

	if (!parent)
		tab = &init_fdtab;
	else
		tab = uk_thread_uktls_var(parent, active_fdtab);
	uk_refcount_acquire(&tab->refcnt);
	uk_thread_uktls_var(child, active_fdtab) = tab;
	return 0;
}

static void fdtab_thread_term(struct uk_thread *child)
{
	struct uk_fdtab *tab = uk_thread_uktls_var(child, active_fdtab);

	/* If a thread has acquired an fdtab ref over its life, release it */
	if (tab && uk_refcount_release(&tab->refcnt))
		fdtab_free(tab);
}

UK_THREAD_INIT(fdtab_thread_init, fdtab_thread_term);

#if CONFIG_LIBPOSIX_PROCESS_CLONE
static int fdtab_clone(const struct clone_args *cl_args,
		       size_t cl_args_len __unused,
		       struct uk_thread *child,
		       struct uk_thread *parent)
{
	struct uk_fdtab *tab = uk_thread_uktls_var(parent, active_fdtab);
	struct uk_fdtab *newtab;

	UK_ASSERT(tab); /* Do not call clone from raw threads */
	if (cl_args->flags & CLONE_FILES) {
		/* Inherit parent's fdtab */
		/* As a compat stop-gap, the raw thread already inherited the
		 * parent's fdtab ref; we don't need to do anything.
		 *
		 * TODO: move inheritance here once stopgap is removed.
		 */
		UK_ASSERT(uk_thread_uktls_var(child, active_fdtab) == tab);
		return 0;
	} else {
		/* Duplicate parent's fdtab */
		newtab = fdtab_duplicate(tab);
		if (unlikely(!newtab))
			return -ENOMEM;
	}
	uk_thread_uktls_var(child, active_fdtab) = newtab;
	return 0;
}

UK_POSIX_CLONE_HANDLER(CLONE_FILES, 0, fdtab_clone, 0);

#endif /* CONFIG_LIBPOSIX_PROCESS_CLONE */
#endif /* CONFIG_LIBPOSIX_FDTAB_MULTITAB */

/* Init fdtab as early as possible, to enable functions that rely on fds */
uk_lib_initcall_prio(init_posix_fdtab, 0x0, UK_LIBPOSIX_FDTAB_INIT_PRIO);
/* Place fd cleanup to run latest before any rootfs terminators */
uk_rootfs_initcall_prio(0x0, term_posix_fdtab, UK_PRIO_LATEST);

/* Internal Syscalls */

int uk_sys_close(int fd)
{
	void *p;
	struct fdval v;

	p = uk_fmap_take(&active_fdtab->fmap, fd);
	if (!p)
		return -EBADF;
	v = fdtab_decode(p);
	file_rel(v.p, v.flags);
	return 0;
}

int uk_sys_dup3(int oldfd, int newfd, int flags)
{
	int r __maybe_unused;
	struct fdval dup;
	void *prevp;
	const void *newent;

	if (oldfd == newfd)
		return -EINVAL;
	if (oldfd < 0 || oldfd >= UK_FDTAB_SIZE ||
	    newfd < 0 || newfd >= UK_FDTAB_SIZE)
		return -EBADF;
	if (flags & ~O_CLOEXEC)
		return -EINVAL;

	dup = _fdtab_get(active_fdtab, oldfd);
	if (!dup.p)
		return -EBADF; /* oldfd not open */
	dup.flags &= ~UK_FDTAB_CLOEXEC;
	dup.flags |= flags ? UK_FDTAB_CLOEXEC : 0;

	prevp = NULL;
	newent = fdtab_encode(dup.p, dup.flags);
	r = uk_fmap_xchg(&active_fdtab->fmap, newfd, newent, &prevp);
	UK_ASSERT(!r); /* newfd should be in range */
	if (prevp) {
		struct fdval prevv = fdtab_decode(prevp);

		file_rel(prevv.p, prevv.flags);
	}
	return newfd;
}

int uk_sys_dup2(int oldfd, int newfd)
{
	if (oldfd == newfd)
		if (uk_fmap_lookup(&active_fdtab->fmap, oldfd))
			return newfd;
		else
			return -EBADF;
	else
		return uk_sys_dup3(oldfd, newfd, 0);
}

int uk_sys_dup_min(int oldfd, int min, int flags)
{
	struct fdval dup;
	const void *newent;
	int fd;

	if (oldfd < 0)
		return -EBADF;
	if (flags & ~O_CLOEXEC)
		return -EINVAL;

	dup = _fdtab_get(active_fdtab, oldfd);
	if (!dup.p)
		return -EBADF;
	dup.flags &= ~UK_FDTAB_CLOEXEC;
	dup.flags |= flags ? UK_FDTAB_CLOEXEC : 0;

	newent = fdtab_encode(dup.p, dup.flags);
	fd = uk_fmap_put(&active_fdtab->fmap, newent, min);
	if (fd >= UK_FDTAB_SIZE) {
		file_rel(dup.p, dup.flags);
		return -ENFILE;
	}
	return fd;
}

int uk_sys_dup(int oldfd)
{
	return uk_sys_dup_min(oldfd, 0, 0);
}

/* Userspace Syscalls */

UK_SYSCALL_R_DEFINE(int, close, int, fd)
{
	return uk_sys_close(fd);
}

UK_SYSCALL_R_DEFINE(int, dup, int, oldfd)
{
	return uk_sys_dup(oldfd);
}

UK_SYSCALL_R_DEFINE(int, dup2, int, oldfd, int, newfd)
{
	return uk_sys_dup2(oldfd, newfd);
}

UK_SYSCALL_R_DEFINE(int, dup3, int, oldfd, int, newfd, int, flags)
{
	return uk_sys_dup3(oldfd, newfd, flags);
}
