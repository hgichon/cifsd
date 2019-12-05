/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *   Copyright (C) 2019 Samsung Electronics Co., Ltd.
 */

#ifndef __CIFSD_TIME_WRAPPERS_H
#define __CIFSD_TIME_WRAPPERS_H

/*
 * A bunch of ugly hacks to workaoround all the API differences
 * between different kernel versions.
 */

#define NTFS_TIME_OFFSET	((u64)(369*365 + 89) * 24 * 3600 * 10000000)

/* Convert the Unix UTC into NT UTC. */
static inline u64 cifs_UnixTimeToNT(struct timespec t)
{
	/* Convert to 100ns intervals and then add the NTFS time offset. */
	return (u64) t.tv_sec * 10000000 + t.tv_nsec / 100 + NTFS_TIME_OFFSET;
}

static inline struct timespec cifs_NTtimeToUnix(__le64 ntutc)
{
	struct timespec ts;
	u64 t;

	/* Subtract the NTFS time offset, then convert to 1s intervals. */
	t = le64_to_cpu(ntutc) - NTFS_TIME_OFFSET;
	ts.tv_nsec = do_div(t, 10000000) * 100;
	ts.tv_sec = t;
	return ts;
}

static inline struct timespec64 to_kern_timespec(struct timespec ts)
{
	return timespec_to_timespec64(ts);
}

static inline struct timespec from_kern_timespec(struct timespec64 ts)
{
	return timespec64_to_timespec(ts);
}

#define CIFSD_TIME_TO_TM	time64_to_tm

static inline long long cifsd_systime(void)
{
	struct timespec		ts;

	getnstimeofday(&ts);
	return cifs_UnixTimeToNT(ts);
}
#endif /* __CIFSD_TIME_WRAPPERS_H */
