#include <stdio.h>
#include <termio.h>

int getch() {
    int ch;

    struct termios buf, save;
    tcgetattr(0, &save);
    buf = save;

    buf.c_lflag &= ~(ICANON|ECHO);

    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;

    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();

    tcsetattr(0, TCSAFLUSH, &save);

    return ch;
}

int main() {
	int ch;

	while (1) {
		ch = getch();
		printf("Character: %d \n", ch);
	}

	return 0;
}

