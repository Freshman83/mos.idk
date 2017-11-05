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

	fd_file = open("test.bin", O_RDWR | O_CREAT);
	if (fd_file == -1)
		perror("Error");

	struct can_frame frame;
	frame.can_id  = 0x001;
	frame.can_dlc = 8;       
	frame.data[0] = 0x01;    
	frame.data[1] = 0x00;   
	frame.data[2] = 0x00;   
	frame.data[3] = 0x00;
	frame.data[4] = 0x00;
	frame.data[5] = 0x00;
	frame.data[6] = 0x00;
	frame.data[7] = 0x00;

	while (true) 
	{
		usleep(200 * 1000);
		error = write(fd_file, &frame, sizeof(struct can_frame));
		if (error == -1)
		{
			perror("An error occured");
			return false;
		}

		frame = empty;
		i += 1;
		
		frame.can_id  = 0x001 + i;   
		frame.can_dlc = 8;    
		frame.data[0] = 0x01 + i;    
		frame.data[1] = 0x00 + i;   
		frame.data[2] = 0x00 + i;   
		frame.data[3] = 0x00 + i;
		frame.data[4] = 0x00 + i;
		frame.data[5] = 0x00 + i;
		frame.data[6] = 0x00 + i;
		frame.data[7] = 0x00 + i;
		
		if (i == 5) {
			return false;
		}
	}

	// Programm beenden
	return 0;
}
