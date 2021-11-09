#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>


#define DEV_PATH "/dev/input/by-path/platform-i8042-serio-0-event-kbd"
#define NUMBER_OF_EVENTS 2


const char* const evtypes[2] = {
        "RELEASED",
        "PRESSED "
};


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
			printf("%s 0x%04x (%d)\n", evtypes[ev.value], ev.code, ev.code);
		}
	}

	return 0;
}

