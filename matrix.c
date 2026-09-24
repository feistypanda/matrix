
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "ansi.h"
#include "util.h"

typedef struct drop {
	int x;
	int y;
	int trailLength;
} drop;

int randomDropY (int height);
int randomDropTailLength (int height);

drop* innitializeDrops (int width, int height);
void displayDrops (drop* drops, int width, int height, int red, int green, int blue, int showFPS);
void updateDrops (drop* drops, int width, int height);
void displayFPS (struct timeval* then, struct timeval* now);
int maxInt (int a, int b);

const int maxWidth = 500;

int main (int argc, char *argv[]) {

	int red = 0, green = 0, blue = 0;
	getRGBFromInput(argc, argv, &red, &green, &blue);

	int width, height;
	getDimensions(&width, &height);

	int sleepTime;
	getSleepTimeFromInput(argc, argv, &sleepTime);

	int showFPS = 0;
	getShowFPSFromInput(argc, argv, &showFPS);

	drop* drops = innitializeDrops(width, height);
	
	struct timeval then, now;
	gettimeofday(&then, NULL);

	background(0, 0, 0);
	clear();

	while (1) {
		
		hideCursor();
		
		getDimensions(&width, &height);

		updateDrops(drops, width, height);

		// So that we overwrite what was already on the screen
		setCursorPos(1, 1);

		if (showFPS) 
			displayFPS(&then, &now);

		displayDrops(drops, width, height, red, green, blue, showFPS);

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

void displayFPS (struct timeval* then, struct timeval* now) {
	gettimeofday(now, NULL);
		
	float timeElapsed = (now->tv_sec - then->tv_sec) * 1000 + (now->tv_usec - then->tv_usec)/1000;

	printf("\e[m%6.3f", timeElapsed);
	*then = *now;
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

void colorText (int y, drop drop, int red, int green, int blue) {
	if (y == drop.y - 1) {
		foreground(215 + red * 40, 215 + green * 40, 215 + blue * 40);
	} else {
		int distFromEnd = (y - (drop.y - drop.trailLength));
		int bright = (int) (((float) distFromEnd) / ((float) drop.trailLength) * 255);

		foreground(red * bright, green * bright, blue * bright);
	}
}

void displayDrops (drop* drops, int width, int height, int red, int green, int blue, int showFPS) {

	for (int y = 0; y < height; y ++) {
		for (int x = 0; x < minInt(width, maxWidth); x ++) {

			if (showFPS && y == 0 && x <= 7) {
				continue;
			}

			if (y < drops[x].y && y >= drops[x].y - drops[x].trailLength) {
				colorText(y, drops[x], red, green, blue);
				fputs(randomChar(), stdout);
			}
			else {
				fputs(" ", stdout);
			}
		}
	}

	fflush(stdout);
}
