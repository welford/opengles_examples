#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include "keymap.h"

int main (int argc, char *argv[])
{
	struct input_event ev[64];
	int fd, rd, value, size = sizeof (struct input_event);
	char name[256] = "Unknown";
	char *device = NULL;
	int idx=0;

	//Setup check
	if (argv[1] == NULL){
		printf("\n");
		exit (0);
	}
	if ((getuid ()) != 0)
		printf ("You are not root! This may not work...n");

	if (argc > 1)
		device = argv[1];

	//Open Device
	if ((fd = open (device, O_RDONLY|O_NONBLOCK)) == -1)
		printf ("%s is not a vaild device.n", device);
		return 1;

	//Print Device Name
	ioctl (fd, EVIOCGNAME (sizeof (name)), name);
	printf ("Reading From : %s (%s)\n", device, name);

	while (1){
		while((rd = read (fd, ev, size * 64)) < size){ } 
		idx=0;
		printf ("size %d\n", rd);
		while(rd>0)
		{
			printf ("%2d:type %d, code %d, value %d\n", idx, ev[idx].type, ev[idx].code, ev[idx].value);
			idx++;
			rd -= size;
		}
	} 
	return 0;
} 