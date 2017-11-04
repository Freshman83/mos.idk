// --------------------------------------------------------------------------
// CanParser - Aufgabe 1
//
// Bearbeitet von:
//     Name, Matrikelnummer
//     Name, Matrikelnummer
//
// --------------------------------------------------------------------------

// Lokale Header
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit()
#include <unistd.h>     // write()
#include <linux/can.h>  // struct can_frame
#include "../lib/can.h" // can_open()

int main (void)
{
	// Socket mit dem angegebenen CAN-Interface öffnen, danach kann
	// mit read() und write() auf das Interface zugegriffen werden.
	int fd_can;
	fd_can = can_open("vcan0");

	// Diese Datenstruktur enthält eine einzelne CAN-Nachricht,
	// deren Inhalt wir nach Belieben festlegen können.
	struct can_frame frame;
	frame.can_id  = 0x123;
	frame.can_dlc = 4;       // Anzahl der Datenbytes.
	frame.data[0] = 0x00;    // Datenbyte 1
	frame.data[1] = 0x01;    // Datenbyte 2
	frame.data[2] = 0x02;    // ..
	frame.data[3] = 0x03;

	// Mithilfe des Befehls write() können eine einzelne Nachricht
	// an das CAN-Interface senden, als Inhalt verwenden wir dazu
	// unsere vorher angelegte Datenstruktur.
	write(fd_can, &frame, sizeof(struct can_frame));

	// Programm beenden
	return 0;
}
