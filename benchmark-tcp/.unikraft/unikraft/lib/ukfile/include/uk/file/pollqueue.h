/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright (c) 2023, Unikraft GmbH and The Unikraft Authors.
 * Licensed under the BSD-3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 */

/* Multi-event poll/wait queue with update chaining support */

#ifndef __UKFILE_POLLQUEUE_H__
#define __UKFILE_POLLQUEUE_H__

#include <uk/config.h>

#include <uk/assert.h>
#include <uk/atomic.h>
#include <uk/rwlock.h>
#include <uk/plat/time.h>
#include <uk/thread.h>

/*
 * Bitmask of event flags.
 *
 * Should be large enough to accomodate what userspace will use as event flags
 * in the least significant bits, along with Unikraft-internal flags (if any)
 * in the more significant bits.
 */
typedef unsigned int uk_pollevent;

struct uk_file;

#if CONFIG_LIBUKFILE_POLLED
/**
 * Callback that fetches events in `mask` currently set on file `f`.
 *
 * This function cannot (meaningfully) fail, must not block indefinitely, and
 * should avoid taking locks or yielding execution when possible.
 *
 * Drivers may choose to not provide this callback, in which case they are
 * responsible for updating the current event levels with `uk_pollq_set`,
 * `uk_pollq_clear`, and/or `uk_pollq_assign` in-band with I/O operations.
 *
 * If drivers do provide this, it will be called every time the instantaneous
 * level of events is queried. Drivers are then responsible only for notifying
 * the rising edges of events via `uk_pollq_set`.
 *
 * @param f File to fetch events for.
 * @param mask Bitmask of events to fetch.
 *
 * @return
 *   Bitwise AND between `mask` and the presently set events on `f`
 */
typedef uk_pollevent (*uk_poll_func)(const struct uk_file *f,
				     uk_pollevent mask);
#endif /* CONFIG_LIBUKFILE_POLLED */

/**
 * Ticket for registering on the poll waiting list.
 *
 * If the newly set events overlap with those in `mask`, wake up `thread`.
 * Tickets are atomically released from the wait queue when waking.
 */
struct uk_poll_ticket {
	struct uk_poll_ticket *next;
	struct uk_thread *thread; /* Thread to wake up */
	uk_pollevent mask; /* Events to register for */
};

#if CONFIG_LIBUKFILE_CHAINUPDATE

/* Update chaining */

enum uk_poll_chain_type {
	UK_POLL_CHAINTYPE_UPDATE,
	UK_POLL_CHAINTYPE_CALLBACK
};

enum uk_poll_chain_op {
	UK_POLL_CHAINOP_CLEAR,
	UK_POLL_CHAINOP_SET
};

struct uk_poll_chain;

/**
 * Update chaining callback function; called on event propagations.
 *
 * @param ev The events that triggered this update.
 * @param op Whether `events` are being set or cleared.
 * @param tick The update chaining ticket this callback is registered with.
 */
typedef void (*uk_poll_chain_callback_fn)(uk_pollevent ev,
					  enum uk_poll_chain_op op,
					  struct uk_poll_chain *tick);

/**
 * Ticket for registering on the update chaining list.
 *
 * If newly modified events overlap with those in `mask`, perform a chain update
 * of these overlapping bits according to `type`:
 *   - UK_POLL_CHAINTYPE_UPDATE: propagate events to `queue`.
 *     If `set` != 0 set/clear events in `set`, instead of original
 *   - UK_POLL_CHAINTYPE_CALLBACK: call `callback`
 */
struct uk_poll_chain {
	struct uk_poll_chain *next;
	uk_pollevent mask; /* Events to register for */
	enum uk_poll_chain_type type;
	union {
		struct {
			struct uk_pollq *queue; /* Where to propagate updates */
			uk_pollevent set; /* Events to set */
		};
		struct {
			uk_poll_chain_callback_fn callback;
			void *arg;
		};
	};
};

/* See comment for main queue below on initializers vs initial values */

/* Initializer for a chain ticket that propagates events to another queue */
#define UK_POLL_CHAIN_UPDATE_INITIALZER(msk, to, ev) { \
	.next = NULL, \
	.mask = (msk), \
	.type = UK_POLL_CHAINTYPE_UPDATE, \
	.queue = (to), \
	.set = (ev) \
}
#define UK_POLL_CHAIN_UPDATE(msk, to, ev) ((struct uk_poll_chain) \
	UK_POLL_CHAIN_UPDATE_INITIALZER((msk), (to), (ev)))

/* Initializer for a chain ticket that calls a custom callback */
#define UK_POLL_CHAIN_CALLBACK_INITIALIZER(msk, cb, dat) { \
	.next = NULL, \
	.mask = (msk), \
	.type = UK_POLL_CHAINTYPE_CALLBACK, \
	.callback = (cb), \
	.arg = (dat) \
}
#define UK_POLL_CHAIN_CALLBACK(msk, cb, dat) ((struct uk_poll_chain) \
	UK_POLL_CHAIN_CALLBACK_INITIALIZER((msk), (cb), (dat)))

#endif /* CONFIG_LIBUKFILE_CHAINUPDATE */

/* Main queue */
struct uk_pollq {
	/* Notification lists */
	struct uk_poll_ticket *wait; /* Polling threads */
	struct uk_poll_ticket **waitend;
#if CONFIG_LIBUKFILE_CHAINUPDATE
	struct uk_poll_chain *prop; /* Registrations for chained updates */
	struct uk_poll_chain **propend;
#endif /* CONFIG_LIBUKFILE_CHAINUPDATE */

	/* Events */
#if CONFIG_LIBUKFILE_POLLED
	uk_poll_func poll; /* If provided, used instead of reading .events */
#endif /* CONFIG_LIBUKFILE_POLLED */
	volatile uk_pollevent events; /* Instantaneous event levels */
	uk_pollevent waitmask; /* Events waited on by threads */
#if CONFIG_LIBUKFILE_CHAINUPDATE
	uk_pollevent propmask; /* Events registered for chaining */
#endif /* CONFIG_LIBUKFILE_CHAINUPDATE */
	/* Locks & sundry */
#if CONFIG_LIBUKFILE_CHAINUPDATE
	void *_tag; /* Internal use */
	struct uk_rwlock proplock; /* Chained updates list lock */
#endif /* CONFIG_LIBUKFILE_CHAINUPDATE */
	struct uk_rwlock waitlock; /* Wait list lock */
};

/*
 * Pollqueues come in two varieties: edge- and level-notified.
 * Edge-notified queues require drivers to only notify rising edges of events,
 * while providing a callback for fetching instantaneous levels.
 * Level-notified queues require drivers to notify both rising and falling edges
 * of events, with the queue itself maintaining event levels.
 * See description of `uk_poll_func` for more details.
 *
 * Edge-notified queues require setting LIBUKFILE_POLLED during configuration.
 */
/*
 * We define initializers separate from an initial values.
 * The former can only be used in (static) variable initializations, while the
 * latter is meant for assigning to variables or as anonymous data structures.
 */
#if CONFIG_LIBUKFILE_CHAINUPDATE
#if CONFIG_LIBUKFILE_POLLED
#define _POLLQ_INIT(q, pollfunc, ev) { \
	.wait = NULL, \
	.waitend = &(q).wait, \
	.prop = NULL, \
	.propend = &(q).prop, \
	.poll = (pollfunc), \
	.events = (ev), \
	.waitmask = 0, \
	.propmask = 0, \
	.proplock = UK_RWLOCK_INITIALIZER((q).proplock, 0), \
	.waitlock = UK_RWLOCK_INITIALIZER((q).waitlock, 0), \
}
#else /* !CONFIG_LIBUKFILE_POLLED */
#define _POLLQ_INIT(q, pollfunc, ev) { \
	.wait = NULL, \
	.waitend = &(q).wait, \
	.prop = NULL, \
	.propend = &(q).prop, \
	.events = (ev), \
	.waitmask = 0, \
	.propmask = 0, \
	.proplock = UK_RWLOCK_INITIALIZER((q).proplock, 0), \
	.waitlock = UK_RWLOCK_INITIALIZER((q).waitlock, 0), \
}
#endif /* !CONFIG_LIBUKFILE_POLLED */
#else /* !CONFIG_LIBUKFILE_CHAINUPDATE */
#if CONFIG_LIBUKFILE_POLLED
#define _POLLQ_INIT(q, pollfunc, ev) { \
	.wait = NULL, \
	.waitend = &(q).wait, \
	.poll = (pollfunc), \
	.events = (ev), \
	.waitmask = 0, \
	.waitlock = UK_RWLOCK_INITIALIZER((q).waitlock, 0), \
}
#else /* !CONFIG_LIBUKFILE_POLLED */
#define _POLLQ_INIT(q, pollfunc, ev) { \
	.wait = NULL, \
	.waitend = &(q).wait, \
	.events = (ev), \
	.waitmask = 0, \
	.waitlock = UK_RWLOCK_INITIALIZER((q).waitlock, 0), \
}
#endif /* !CONFIG_LIBUKFILE_POLLED */
#endif /* !CONFIG_LIBUKFILE_CHAINUPDATE */

#if CONFIG_LIBUKFILE_POLLED
#define UK_POLLQ_EDGE_INITIALIZER(q, pollfunc) _POLLQ_INIT(q, pollfunc, 0)

#define UK_POLLQ_EDGE_INIT_VALUE(q, pollfunc) \
	((struct uk_pollq)UK_POLLQ_EDGE_INITIALIZER(q, pollfunc))
#endif /* CONFIG_LIBUKFILE_POLLED */

#define UK_POLLQ_LEVEL_EVENTS_INITIALIZER(q, ev) _POLLQ_INIT(q, NULL, ev)
#define UK_POLLQ_LEVEL_INITIALIZER(q) UK_POLLQ_LEVEL_EVENTS_INITIALIZER(q, 0)

#define UK_POLLQ_LEVEL_EVENTS_INIT_VALUE(q, ev) \
	((struct uk_pollq)UK_POLLQ_LEVEL_EVENTS_INITIALIZER(q, ev))
#define UK_POLLQ_LEVEL_INIT_VALUE(q) \
	((struct uk_pollq)UK_POLLQ_LEVEL_INITIALIZER(q))

/* Polling cancellation */

/**
 * Remove a specific `ticket` from the wait list.
 */
static inline
void uk_pollq_cancel_ticket(struct uk_pollq *q, struct uk_poll_ticket *ticket)
{
	uk_rwlock_wlock(&q->waitlock);
	for (struct uk_poll_ticket **p = &q->wait; *p; p = &(*p)->next)
		if (*p == ticket) {
			*p = ticket->next;
			ticket->next = NULL;
			if (!*p)
				q->waitend = p;
			break;
		}
	uk_rwlock_wunlock(&q->waitlock);
}

/**
 * Remove the ticket of a specific `thread` from the wait list.
 */
static inline
void uk_pollq_cancel_thread(struct uk_pollq *q, struct uk_thread *thread)
{
	uk_rwlock_wlock(&q->waitlock);
	for (struct uk_poll_ticket **p = &q->wait; *p; p = &(*p)->next) {
		struct uk_poll_ticket *t = *p;

		if (t->thread == thread) {
			*p = t->next;
			t->next = NULL;
			if (!*p)
				q->waitend = p;
			break;
		}
	}
	uk_rwlock_wunlock(&q->waitlock);
}

/**
 * Remove the ticket of the current thread from the wait list.
 */
#define uk_pollq_cancel(q) uk_pollq_cancel_thread((q), uk_thread_current())

/* Polling */

#if CONFIG_LIBUKFILE_POLLED
/**
 * INTERNAL. Poll for the events in `req`; never block or take locks,
 * always return immediately.
 */
static inline
uk_pollevent _pollq_poll_immediate(struct uk_pollq *q, uk_pollevent req)
{
	return q->poll ? 0 : q->events & req;
}

/**
 * INTERNAL. Poll for the events in `req` with the waitlock held; may block.
 */
static inline
uk_pollevent _pollq_poll_locked(struct uk_pollq *q, uk_pollevent req,
				const struct uk_file *f)
{
	return q->poll ? q->poll(f, req) : q->events & req;
}
#else /* !CONFIG_LIBUKFILE_POLLED */
/**
 * INTERNAL. Poll for the events in `req`; never block or take locks,
 * always return immediately.
 */
static inline
uk_pollevent _pollq_poll_immediate(struct uk_pollq *q, uk_pollevent req)
{
	return q->events & req;
}

/**
 * INTERNAL. Poll for the events in `req` with the waitlock held; may block.
 */
static inline
uk_pollevent _pollq_poll_locked(struct uk_pollq *q, uk_pollevent req,
				const struct uk_file *f __unused)
{
	return _pollq_poll_immediate(q, req);
}
#endif

/**
 * INTERNAL. Atomically poll & lock if required.
 *
 * @param q Target queue.
 * @param req Events to poll for.
 * @param exp Events expected to be already set.
 *
 * @return
 *   non-zero evmask with lock released if events appeared
 *   0 with lock held otherwise.
 */
static inline
uk_pollevent _pollq_lock(struct uk_pollq *q, uk_pollevent req,
			 uk_pollevent exp, const struct uk_file *f)
{
	uk_pollevent ev;

	uk_rwlock_rlock(&q->waitlock);
	/* Check if events were set while acquiring the lock */
	if ((ev = _pollq_poll_locked(q, req, f) & ~exp))
		uk_rwlock_runlock(&q->waitlock);
	return ev;
}

/**
 * INTERNAL. Wait for events until a timeout.
 *
 * Must be called only after `_pollq_lock` returns 0.
 *
 * @param q Target queue.
 * @param req Events to poll for.
 * @param deadline Max number of nanoseconds to wait or, or 0 if forever
 *
 * @return
 *   0 on timeout
 *   non-zero if awoken
 */
static inline
int _pollq_wait(struct uk_pollq *q, uk_pollevent req, __nsec deadline)
{
	struct uk_poll_ticket **tail;
	struct uk_thread *__current;
	struct uk_poll_ticket tick;
	int timeout;

	/* Mark request in waitmask */
	(void)uk_or(&q->waitmask, req);
	/* Compete to register */

	__current = uk_thread_current();
	tick = (struct uk_poll_ticket){
		.next = NULL,
		.thread = __current,
		.mask = req,
	};
	tail = uk_exchange_n(&q->waitend, &tick.next);
	/* tail is ours alone, safe to link in */
	UK_ASSERT(!*tail); /* Should be a genuine list tail */
	*tail = &tick;

	/* Block until awoken */
	uk_thread_block_until(__current, deadline);
	uk_rwlock_runlock(&q->waitlock);
	uk_sched_yield();
	/* Back, wake up, check if timed out & try again */
	timeout = deadline && ukplat_monotonic_clock() >= deadline;
	if (timeout)
		uk_pollq_cancel_ticket(q, &tick);
	return !timeout;
}

/**
 * Poll for the events in `req`, returning the present levels of events.
 *
 * May yield execution or acquire locks, but will never block indefinitely.
 *
 * @param q Target queue.
 * @param req Events to poll for.
 * @param f File to poll for events, in case of an edge-triggered `q`.
 *
 * @return
     Bitwise AND between `req` and the events set in `q`
 */
static inline
uk_pollevent uk_pollq_poll_level(struct uk_pollq *q, uk_pollevent req,
				 const struct uk_file *f __maybe_unused)
{
	uk_pollevent ev;

	if ((ev = _pollq_poll_immediate(q, req)))
		return ev;
#if CONFIG_LIBUKFILE_POLLED
	if (q->poll && !(ev = _pollq_lock(q, req, 0, f)))
		uk_rwlock_runlock(&q->waitlock);
#endif /* CONFIG_LIBUKFILE_POLLED */
	return ev;
}

/**
 * Poll for the events in `req`, blocking until `deadline` or an event is set.
 *
 * @param q Target queue.
 * @param req Events to poll for.
 * @param deadline Max number of nanoseconds to wait for, or 0 if forever
 *
 * @return
 *   Bitwise AND between `req` and the events set in `q`, or 0 if timed out
 */
static inline
uk_pollevent uk_pollq_poll_until(struct uk_pollq *q, uk_pollevent req,
				 __nsec deadline, const struct uk_file *f)
{
	uk_pollevent ev;

	do {
		if ((ev = _pollq_poll_immediate(q, req)))
			return ev;
		if ((ev = _pollq_lock(q, req, 0, f)))
			return ev;
	} while (_pollq_wait(q, req, deadline));
	return ev;
}

/**
 * Poll for the events in `req`, blocking until an event is set.
 *
 * @param q Target queue.
 * @param req Events to poll for.
 *
 * @return
 *   Bitwise AND between `req` and the events set in `q`
 */
#define uk_pollq_poll(q, req, f) uk_pollq_poll_until(q, req, 0, f)

#if CONFIG_LIBUKFILE_CHAINUPDATE
/* Propagation */

/**
 * INTERNAL. Register update chaining ticket.
 *
 * Must be called with appropriate locks held
 *
 * @param q Target queue.
 * @param tick Update chaining ticket to register.
 */
static inline
void _pollq_register(struct uk_pollq *q, struct uk_poll_chain *tick)
{
	struct uk_poll_chain **tail;

	(void)uk_or(&q->propmask, tick->mask);
	tail = uk_exchange_n(&q->propend, &tick->next);
	UK_ASSERT(!*tail); /* Should be genuine list tail */
	*tail = tick;
}

/**
 * Register ticket `tick` for event propagations on `q`.
 *
 * @param q Target queue.
 * @param tick Update chaining ticket to register.
 */
static inline
void uk_pollq_register(struct uk_pollq *q, struct uk_poll_chain *tick)
{
	uk_rwlock_rlock(&q->proplock);
	_pollq_register(q, tick);
	uk_rwlock_runlock(&q->proplock);
}

/**
 * Unregister ticket `tick` from event propagations on `q`.
 *
 * @param q Target queue.
 * @param tick Update chaining ticket to unregister.
 */
static inline
void uk_pollq_unregister(struct uk_pollq *q, struct uk_poll_chain *tick)
{
	uk_rwlock_wlock(&q->proplock);
	for (struct uk_poll_chain **p = &q->prop; *p; p = &(*p)->next)
		if (*p == tick) {
			*p = tick->next;
			tick->next = NULL;
			if (!*p) /* We unlinked last node */
				q->propend = p;
			break;
		}
	uk_rwlock_wunlock(&q->proplock);
}

/**
 * Poll for events and/or register for propagation on `q`.
 *
 * @param q Target queue.
 * @param tick Update chaining ticket to register, if needed.
 * @param force If 0, will immediately return without registering if any of the
 *   requested events are set. If non-zero, always register.
 *
 * @return
 *   Requested events that are currently active.
 */
static inline
uk_pollevent uk_pollq_poll_register(struct uk_pollq *q,
				    struct uk_poll_chain *tick, int force,
				    const struct uk_file *f)
{
	uk_pollevent ev;
	uk_pollevent req = tick->mask;

	if (!force && (ev = _pollq_poll_immediate(q, req)))
		return ev;
	/* Might need to register */
	uk_rwlock_rlock(&q->proplock);
	if ((ev = _pollq_poll_locked(q, req, f)) && !force)
		goto out;
	_pollq_register(q, tick);
out:
	uk_rwlock_runlock(&q->proplock);
	return ev;
}
#endif /* CONFIG_LIBUKFILE_CHAINUPDATE */

/* Updating */

/**
 * Update events, setting those in `set` and handling notifications.
 *
 * @param q Target queue.
 * @param set Events to set.
 * @param n Maximum number of threads to wake up. If < 0 wake up all threads.
 *   Chained updates have their own defined notification semantics and may
 *   notify more threads than specified in `n`.
 *
 * @return
 *   The previous event set.
 */
uk_pollevent uk_pollq_set_n(struct uk_pollq *q, uk_pollevent set, int n);

/**
 * Update events, clearing those in `clr`.
 *
 * Only available on level-triggered queues.
 *
 * @param q Target queue.
 * @param clr Events to clear.
 *
 * @return
 *   The previous event set.
 */
uk_pollevent uk_pollq_clear(struct uk_pollq *q, uk_pollevent clr);

/**
 * Replace the events in `q` with `val` and handle notifications.
 *
 * Only available on level-triggered queues.
 *
 * @param q Target queue.
 * @param val New event set.
 * @param n Maximum number of threads to wake up. If < 0 wake up all threads.
 *   Chained updates have their own defined notification semantics and may
 *   notify more threads than specified in `n`
 *
 * @return
 *   The previous event set.
 */
uk_pollevent uk_pollq_assign_n(struct uk_pollq *q, uk_pollevent val, int n);

#define UK_POLLQ_NOTIFY_ALL -1

/**
 * Update events, setting those in `set` and handling notifications.
 *
 * @param q Target queue.
 * @param set Events to set.
 *
 * @return
 *   The previous event set.
 */
#define uk_pollq_set(q, s) uk_pollq_set_n(q, s, UK_POLLQ_NOTIFY_ALL)

/**
 * Replace the events in `q` with `val` and handle notifications.
 *
 * Only available on level-triggered queues.
 *
 * @param q Target queue.
 * @param val New event set.
 *
 * @return
 *   The previous event set.
 */
#define uk_pollq_assign(q, s) uk_pollq_assign_n(q, s, UK_POLLQ_NOTIFY_ALL)

#endif /* __UKFILE_POLLQUEUE_H__ */
