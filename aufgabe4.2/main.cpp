// --------------------------------------------------------------------------
// CanParser - Aufgabe 4.1
//
// Bearbeitet von:
//     Sascha Niklas, 257146
//     David Rotärmel, 258201
//
// --------------------------------------------------------------------------

// Lokale Header
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <unistd.h>     // write()
#include <linux/can.h>  // struct can_frame
#include "../lib/can.h" // can_open()
#include <fcntl.h>

int main (void)
{	
	struct can_frame empty;
	unsigned int i = 0;
	int error;
	int fd_file;
	int fd_can;

	fd_file = open("test.bin", O_RDONLY, 0660);
	if (fd_file == -1)
		perror("File Error");

	fd_can = can_open("vcan0");
	if (fd_can == -1)
		perror("CAN Error");

	struct can_frame frame;
	frame.can_id  = 0x001;
	frame.can_dlc = 8;

	int size = 1;
	char data[8];

	while (size != 0)
	{
		size = read(fd_file, data, 8);
		if (size == -1){
			perror("Read Error");
		}
		if (size == 0) {
			printf("End of File\n");
		}

		frame.data[0] = data[0];
		frame.data[1] = data[1];
		frame.data[2] = data[2];
		frame.data[3] = data[3];
		frame.data[4] = data[4];
		frame.data[5] = data[5];
		frame.data[6] = data[6];
		frame.data[7] = data[7];

		error = write(fd_can, &frame, sizeof(struct can_frame));
		if (error == -1)
		{
			perror("Write Error");
			return false;
		}

		frame = empty;
		i += 1;
		usleep(200 * 1000);
	}

	close(fd_file);
	return 0;
}
