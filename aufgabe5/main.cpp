// --------------------------------------------------------------------------
// CanParser - Aufgabe 5
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
#include <fcntl.h>      // O_RDONLY
#include <linux/can.h>  // struct can_frame
#include "../lib/can.h" // can_open()

// Feste Definitionen für dieses Programm
#define CAN_INTERFACE    "vcan0"
#define LOG_FILE         "../logs/20100930-1400_opel-obd-can.raw"

// Diese Datenstruktur repräsentiert einen einzelnen Eintrag in der Log-Datei.
struct log_entry
{
	struct timeval    timeval;
	struct can_frame  can_frame;
};

int main (void)
{
	// Socket mit dem angegebenen CAN-Interface öffnen, danach kann
	// mit read() und write() auf das Interface zugegriffen werden.
	int fd_can;
	fd_can = can_open("vcan0");

	// Log-Datei öffnen. Hier sind die Daten in Form von struct log_entry
	// Einträgen hinterlegt, die nacheinander mit read() lesbar sind.
	int fd_log;
	// fd_log = open(...); <- Hier Ihren Code einfügen!

	// Einzelnen Log-Eintrag auslesen.
	struct log_entry entry;
	read(fd_log, &entry, sizeof(struct log_entry));

	// Eine einzelne Nachricht an das CAN-Interface senden.
	// Als Inhalt dient das can_frame innerhalb der log_entry Struktur.
	write(fd_can, &entry.can_frame, sizeof(struct can_frame));

	// Programm beenden
	return 0;
}
