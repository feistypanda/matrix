
#include <stdio.h>
#include <stdlib.h>

const char clearAnsiEscape[] = "\ec";
const char carriageReturnAnsiEscape = '\r';
const char hideCursorAnsiEscape[] = "\e[?25l";
const char showCursorAnsiEscape[] = "\e[?25h";

static const char *MATRIX_CHARS[] = {
    "ｦ","ｱ","ｳ","ｴ","ｵ","ｶ","ｷ","ｹ","ｺ","ｻ","ｼ","ｽ","ｾ","ｿ",
    "ﾀ","ﾂ","ﾃ","ﾅ","ﾆ","ﾇ","ﾈ","ﾊ","ﾋ","ﾎ","ﾏ","ﾐ","ﾑ","ﾒ",
    "ﾓ","ﾔ","ﾕ","ﾗ","ﾘ","ﾜ",
    "0","1","2","3","4","5","6","7","8","9",
    "Z",":",".","\"","=","*","+","-","<",">","|","ç"
};
#define MATRIX_COUNT (sizeof MATRIX_CHARS / sizeof MATRIX_CHARS[0])

const char* randomChar(void) {
    return MATRIX_CHARS[rand() % MATRIX_COUNT];
}

void clear (void) {
	printf("%s", clearAnsiEscape);
	fflush(stdout);
}

void carriageReturn (void) {
	printf("%c", carriageReturnAnsiEscape);
}

void hideCursor (void) {
	printf("%s", hideCursorAnsiEscape);
}

void showCursor (void) {
	printf("%s", showCursorAnsiEscape);
}

void setCursorPos (int x, int y) {
	printf("\e[%d;%dH", x, y);
}

void background (int r, int g, int b) {
	printf("\e[48;2;%d;%d;%dm", r, g, b);
}

void foreground (int r, int g, int b) {
	printf("\e[38;2;%d;%d;%dm", r, g, b);
}

void foreground256 (int v) {
	printf("\e[38;5;%dm", v);
}