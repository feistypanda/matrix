
#include <errno.h>
#include <time.h>

// Source - https://stackoverflow.com/a/1157217
// Posted by caf, modified by community. See post 'Timeline' for change history
// Retrieved 2026-09-20, License - CC BY-SA 4.0

int msleep(long msec)
{
	struct timespec ts;
	int res;

	if (msec < 0)
	{
		errno = EINVAL;
		return -1;
	}

	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;

	do {
		res = nanosleep(&ts, &ts);
	} while (res && errno == EINTR);

	return res;
}
