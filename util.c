
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

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

void getDimensions (int* width, int* height) {
	struct ttysize ts;
    ioctl(0, TIOCGSIZE, &ts);

    *height = ts.ts_lines;
    *width = ts.ts_cols;
}

void getRGBFromInput (int argc, char* argv[], int* red, int* green, int* blue) {
	if (argc > 1) {
		if (strcmp(argv[1], "green") == 0) {
			*green = 1;
		} else if (strcmp(argv[1], "red") == 0) {
			*red = 1;
		} else if (strcmp(argv[1], "blue") == 0) {
			*blue = 1;
		} else {
			*green = 1;
		}
	} else {
		*green = 1;
	}
}

void getSleepTimeFromInput (int argc, char* argv[], int* sleepTime) {
	*sleepTime = 100;
	
	if (argc > 2) {
		long long res = strtonum(argv[2], 0, 1000, NULL);
		if (res >= 10 && res <= 1000) {
			*sleepTime = res;
		}
	}
}

void getShowFPSFromInput(int argc, char* argv[], int* showFPS) {
	if ((argc > 2 && strcmp(argv[2], "-f") == 0) || (argc > 3 && strcmp(argv[3], "-f") == 0)) {
		*showFPS = 1;
	} else {
		*showFPS = 0;
	}
}