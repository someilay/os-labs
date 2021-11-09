#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>


#define DEV_PATH "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
#define NUMBER_OF_EVENTS 2
#define NUMBER_OF_LETTERS 26


int pressed_letters[NUMBER_OF_LETTERS] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const int pe_shortcut[2] = { 2, 9 };
const int cap_shortcut[3] = { 21, 10, 9 };
const int buy_shortcut[3] = { 23, 6, 5 }; // B + U + Y 


void update_letters(int event_type, int event_code) {
	if (16 <= event_code && event_code <= 25) {
		pressed_letters[event_code - 16] = event_type;
		return;
	}
	if (30 <= event_code && event_code <= 38) {
		pressed_letters[event_code - 20] = event_type;
		return;
	}
	if (44 <= event_code && event_code <= 50) {
		pressed_letters[event_code - 25] = event_type;
		return;
	}
}


void check(const int* shortcut, int length, char* msg) {
	for (int i = 0; i < length; i++) {
		if (!pressed_letters[shortcut[i]]) {
			return;
		}
	}
	printf("\n%s\n", msg);
}


int main() {
	struct input_event ev;
	int fd = open(DEV_PATH, O_RDONLY);

	if (fd == -1) {
		printf("Cannot open file %s\n", DEV_PATH);
		return EXIT_FAILURE;
	}
	while (1) {
		read(fd, &ev, sizeof(ev));

		if (ev.type == EV_KEY && ev.value >= 0 && ev.value < NUMBER_OF_EVENTS) {
			update_letters(ev.value, ev.code);
			check(pe_shortcut, 2, "I passed the Exam!");
			check(cap_shortcut, 3, "Get some cappuccino!");
			check(buy_shortcut, 3, "Goodbuy!");
		}
	}

	return 0;
}

