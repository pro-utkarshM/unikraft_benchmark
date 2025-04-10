/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (c) 2023, Unikraft GmbH and The Unikraft Authors.
 * Licensed under the BSD-3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 */
#define _GNU_SOURCE
/* Userspace shim syscalls that delegate to either posix-fdio or vfscore */

#include <fcntl.h>
#include <sys/ioctl.h>

#include <uk/posix-fdio.h>
#if CONFIG_LIBVFSCORE
#include <vfscore/syscalls.h>
#endif /* CONFIG_LIBVFSCORE */
#include <uk/posix-fdtab.h>
#include <uk/syscall.h>


/* I/O */

UK_SYSCALL_R_DEFINE(ssize_t, preadv2, int, fd, const struct iovec *, iov,
		    int, iovcnt, off_t, offset, int, flags)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_preadv2(sf.ofile, iov, iovcnt, offset, flags);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		if (flags)
			r = -EINVAL;
		else
			r = vfscore_preadv(sf.vfile, iov, iovcnt, offset);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(ssize_t, preadv, int, fd, const struct iovec *, iov,
		    int, iovcnt, off_t, offset)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_preadv(sf.ofile, iov, iovcnt, offset);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_preadv(sf.vfile, iov, iovcnt, offset);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

/*
 * Some libc's define some macros that remove the 64 suffix
 * from some system call function names (e.g., <unistd.h>, <fcntl.h>).
 * We need to undefine them here so that our system call
 * registration does not fail in such a case.
 */
#ifdef pread64
#undef pread64
#endif

UK_SYSCALL_R_DEFINE(ssize_t, pread64, int, fd,
		    void *, buf, size_t, count, off_t, offset)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_pread(sf.ofile, buf, count, offset);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_pread64(sf.vfile, buf, count, offset);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

#if UK_LIBC_SYSCALLS
__alias(pread64, pread);
#endif /* UK_LIBC_SYSCALLS */

UK_SYSCALL_R_DEFINE(ssize_t, readv, int, fd,
		    const struct iovec *, iov, int, iovcnt)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_readv(sf.ofile, iov, iovcnt);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_readv(sf.vfile, iov, iovcnt);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(ssize_t, read, int, fd,
		    void *, buf, size_t, count)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_read(sf.ofile, buf, count);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_read(sf.vfile, buf, count);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(ssize_t, pwritev2, int, fd, const struct iovec*, iov,
		    int, iovcnt, off_t, offset, int, flags)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_pwritev2(sf.ofile, iov, iovcnt, offset, flags);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		if (flags)
			r = -EINVAL;
		else
			r = vfscore_pwritev(sf.vfile, iov, iovcnt, offset);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(ssize_t, pwritev, int, fd, const struct iovec*, iov,
		    int, iovcnt, off_t, offset)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_pwritev(sf.ofile, iov, iovcnt, offset);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_pwritev(sf.vfile, iov, iovcnt, offset);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

/* (see comment on pread64 above) */
#ifdef pwrite64
#undef pwrite64
#endif

UK_SYSCALL_R_DEFINE(ssize_t, pwrite64, int, fd,
		    const void *, buf, size_t, count, off_t, offset)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_pwrite(sf.ofile, buf, count, offset);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_pwrite64(sf.vfile, buf, count, offset);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

#if UK_LIBC_SYSCALLS
__alias(pwrite64, pwrite);
#endif /* UK_LIBC_SYSCALLS */

UK_SYSCALL_R_DEFINE(ssize_t, writev, int, fd, const struct iovec *, iov,
		    int, iovcnt)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_writev(sf.ofile, iov, iovcnt);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_writev(sf.vfile, iov, iovcnt);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(ssize_t, write, int, fd, const void *, buf, size_t, count)
{
	ssize_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_write(sf.ofile, buf, count);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		r = vfscore_write(sf.vfile, buf, count);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(off_t, lseek, int, fd, off_t, offset, int, whence)
{
	off_t r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_lseek(sf.ofile, offset, whence);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
	{
		/* vfscore_lseek returns positive error codes */
		int err = vfscore_lseek(sf.vfile, offset, whence, &r);

		if (err) {
			UK_ASSERT(err > 0);
			r = -err;
		}
		fdrop(sf.vfile);
		break;
	}
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

/* Stat */

UK_SYSCALL_R_DEFINE(int, fstat, int, fd, struct stat *, statbuf)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_fstat(sf.ofile, statbuf);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore_fstat returns positive error codes */
		r = -vfscore_fstat(sf.vfile, statbuf);
		fdrop(sf.vfile);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(int, fchmod, int, fd, mode_t, mode)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_fchmod(sf.ofile, mode);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore_fchmod returns positive error codes */
		r = -vfscore_fchmod(sf.vfile, mode);
		fdrop(sf.vfile);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(int, fchown, int, fd, uid_t, owner, gid_t, group)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_fchown(sf.ofile, owner, group);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore does not support fchown; stub out */
		fdrop(sf.vfile);
		r = 0;
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

/* Ctl */

UK_SYSCALL_R_DEFINE(int, fsync, int, fd)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_fsync(sf.ofile);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore_fsync returns positive error codes */
		r = -vfscore_fsync(sf.vfile);
		fdrop(sf.vfile);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(int, fdatasync, int, fd)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_fdatasync(sf.ofile);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore has no separate fdatasync; use fsync */
		/* vfscore_fsync returns positive error codes */
		r = -vfscore_fsync(sf.vfile);
		fdrop(sf.vfile);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(int, ftruncate, int, fd, off_t, len)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_ftruncate(sf.ofile, len);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore_fallocate returns positive error codes */
		r = -vfscore_ftruncate(sf.vfile, len);
		fdrop(sf.vfile);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(int, fallocate, int, fd, int, mode, off_t, off, off_t, len)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_fallocate(sf.ofile, mode, off, len);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore_fallocate returns positive error codes */
		r = -vfscore_fallocate(sf.vfile, mode, off, len);
		fdrop(sf.vfile);
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_SYSCALL_R_DEFINE(int, fadvise64,
		    int, fd, off_t, off, off_t, len, int, advice)
{
	int r;
	union uk_shim_file sf;

	switch (uk_fdtab_shim_get(fd, &sf)) {
	case UK_SHIM_OFILE:
		r = uk_sys_fadvise(sf.ofile, off, len, advice);
		uk_ofile_release(sf.ofile);
		break;
#if CONFIG_LIBVFSCORE
	case UK_SHIM_LEGACY:
		/* vfscore does not support fadvise; stub out */
		fdrop(sf.vfile);
		r = 0;
		break;
#endif /* CONFIG_LIBVFSCORE */
	default:
		r = -EBADF;
	}
	return r;
}

UK_LLSYSCALL_R_DEFINE(int, fcntl, int, fd,
		      unsigned int, cmd, unsigned long, arg)
{
	int fdflags = 0;

	switch (cmd) {
	case F_DUPFD_CLOEXEC:
		fdflags |= O_CLOEXEC;
		/* Fallthrough */
	case F_DUPFD:
		return uk_sys_dup_min(fd, (int)arg, fdflags);
	case F_GETFD:
		fdflags = uk_fdtab_getflags(fd);
		if (unlikely(fdflags < 0))
			return fdflags;
		return (fdflags & O_CLOEXEC) ? FD_CLOEXEC : 0;
	case F_SETFD:
		return uk_fdtab_setflags(fd,
			((int)arg & FD_CLOEXEC) ? O_CLOEXEC : 0);
	default:
	{
		int r;
		union uk_shim_file sf;

		switch (uk_fdtab_shim_get(fd, &sf)) {
		case UK_SHIM_OFILE:
			r = uk_sys_fcntl(sf.ofile, cmd, arg);
			uk_ofile_release(sf.ofile);
			break;
#if CONFIG_LIBVFSCORE
		case UK_SHIM_LEGACY:
			r = vfscore_fcntl(sf.vfile, cmd, arg);
			fdrop(sf.vfile);
			break;
#endif /* CONFIG_LIBVFSCORE */
		default:
			r = -EBADF;
		}
		return r;
	}
	}
}

#if UK_LIBC_SYSCALLS
int fcntl(int fd, int cmd, ...)
{
	intptr_t arg = 0;
	va_list ap;

	va_start(ap, cmd);
	switch (cmd) {
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_SETFD:
	case F_SETFL:
	case F_SETOWN:
	case F_SETSIG:
	case F_SETLEASE:
	case F_NOTIFY:
	case F_SETPIPE_SZ:
	case F_ADD_SEALS:
		arg = va_arg(ap, int);
		break;
	case F_SETLK:
	case F_SETLKW:
	case F_GETLK:
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
	case F_OFD_GETLK:
	case F_GETOWN_EX:
	case F_SETOWN_EX:
	case F_GET_RW_HINT:
	case F_SET_RW_HINT:
	case F_GET_FILE_RW_HINT:
	case F_SET_FILE_RW_HINT:
		arg = (intptr_t)va_arg(ap, void *);
		break;
	default:
		break;
	}
	va_end(ap);

	return uk_syscall_e_fcntl(fd, cmd, arg);
}
#endif /* UK_LIBC_SYSCALLS */

UK_LLSYSCALL_R_DEFINE(int, ioctl, int, fd, unsigned int, request, void *, arg)
{
	int r;
	union uk_shim_file sf;

	switch (request) {
	case FIOCLEX:
		return uk_fdtab_setflags(fd, O_CLOEXEC);
	case FIONCLEX:
		return uk_fdtab_setflags(fd, 0);
	default:
		switch (uk_fdtab_shim_get(fd, &sf)) {
		case UK_SHIM_OFILE:
			r = uk_sys_ioctl(sf.ofile, request, arg);
			uk_ofile_release(sf.ofile);
			break;
#if CONFIG_LIBVFSCORE
		case UK_SHIM_LEGACY:
			/* vfscore_ioctl returns positive error codes */
			r = -vfscore_ioctl(sf.vfile, request, arg);
			fdrop(sf.vfile);
			break;
#endif /* CONFIG_LIBVFSCORE */
		default:
			r = -EBADF;
		}
	}
	return r;
}

#if UK_LIBC_SYSCALLS
int ioctl(int fd, unsigned long request, ...)
{
	va_list ap;
	void *arg;

	va_start(ap, request);
	arg = va_arg(ap, void*);
	va_end(ap);

	return uk_syscall_e_ioctl((long)fd, (long)request, (long)arg);
}
#endif /* UK_LIBC_SYSCALLS */
