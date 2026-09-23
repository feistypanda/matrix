
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "ansi.h"
#include "sleep.h"

typedef struct drop {
	int x;
	int y;
	int trailLength;
} drop;

int randomDropY (int height);
int randomDropTailLength (int height);

void getDimensions (int* width, int* height);
drop* innitializeDrops (int width, int height);
void displayDrops (drop* drops, int width, int height);
void updateDrops (drop* drops, int width, int height);
int maxInt (int a, int b);

const int maxWidth = 500;

int red = 0;
int green = 0;
int blue = 0;

int main (int argc, char *argv[]) {

	background(0, 0, 0);

	if (argc > 1) {
		if (strcmp(argv[1], "green") == 0) {
			green = 1;
		} else if (strcmp(argv[1], "red") == 0) {
			red = 1;
		} else if (strcmp(argv[1], "blue") == 0) {
			blue = 1;
		}
	} else {
		green = 1;
	}

	clear();
	
	int width;
	int height;
	
	getDimensions(&width, &height);

	drop* drops = innitializeDrops(width, height);

	int sleepTime = 100;
	if (argc > 2) {
		long long res = strtonum(argv[2], 0, 1000, NULL);
		if (res >= 10 && res <= 1000) {
			sleepTime = res;
		}
	}
	
	struct timeval then, now;

	gettimeofday(&then, NULL);

	while (1) {
		hideCursor();
		getDimensions(&width, &height);

		updateDrops(drops, width, height);

		// So that we overwrite what was already on the screen
		setCursorPos(1, 1);

		gettimeofday(&now, NULL);
		
		float timeElapsed = (now.tv_sec - then.tv_sec) * 1000 + (now.tv_usec - then.tv_usec)/1000;

		printf("\e[m%6.3f", timeElapsed);
		then = now;
		
		displayDrops(drops, width, height);

		msleep(sleepTime);
	}

	free(drops);
}

int minInt (int a, int b) {
	if (a < b)
		return a;
	return b;
}

int randomDropY (int height) {
	return 0 - rand() % (height * 4);
}

int randomDropTrailLength (int height) {
	return 4 + rand() % (height);
}

void getDimensions (int* width, int* height) {
	struct ttysize ts;
    ioctl(0, TIOCGSIZE, &ts);

    *height = ts.ts_lines;
    *width = ts.ts_cols;
}

drop* innitializeDrops (int width, int height) {
	drop* drops = malloc(sizeof(drop) * maxWidth);

	// Populate that array with raindrops
	for (int i = 0; i < maxWidth; i ++) {
		drops[i].x = i;
		drops[i].y = randomDropY(height);
		drops[i].trailLength = randomDropTrailLength(height);
	}

	return drops;
}

void updateDrops (drop* drops, int width, int height) {
	for (int i = 0; i < minInt(width, maxWidth); i ++) {
		drops[i].y ++;

		if (drops[i].y - drops[i].trailLength >= height) {
			drops[i].y = randomDropY(height);
			drops[i].trailLength = randomDropTrailLength(height);
		}
	}
}

void colorText (int y, drop drop) {
	if (y == drop.y - 1) {
		foreground(215 + red * 40, 215 + green * 40, 215 + blue * 40);
	} else {
		int distFromEnd = (y - (drop.y - drop.trailLength));
		int bright = (int) (((float) distFromEnd) / ((float) drop.trailLength) * 255);

		foreground(red * bright, green * bright, blue * bright);
	}
}

void displayDrops (drop* drops, int width, int height) {

	for (int y = 0; y < height; y ++) {
		for (int x = 0; x < minInt(width, maxWidth); x ++) {

			if (y == 0 && x <= 7) {
				continue;
			}

			if (y < drops[x].y && y >= drops[x].y - drops[x].trailLength) {
				colorText(y, drops[x]);
				fputs(randomChar(), stdout);
			}
			else {
				fputs(" ", stdout);
			}
		}
	}

	fflush(stdout);
}
