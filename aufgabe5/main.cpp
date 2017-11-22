// --------------------------------------------------------------------------
// CanParser - Aufgabe 5
//
// Bearbeitet von:
//     Sascha Niklas, Matrikelnummer
//     David Rotärmel, Matrikelnummer
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
//#define LOG_FILE         "../logs/20100930-1400_opel-obd-can.raw"
#define LOG_FILE         "../logs/20121011_bmw_stopngo_5min.raw"

struct timeval logTimeOld;
void suspendAccordingToLogTime(int entryCount, struct timeval logTimeNew){
	if(entryCount != 1){
		if((logTimeNew.tv_usec - logTimeOld.tv_usec) >= 1){
			printf("Current Log-Entry: %i\tNow sleeping for %ld\tus\n",
					entryCount, (logTimeNew.tv_usec - logTimeOld.tv_usec));
			usleep( (logTimeNew.tv_usec - logTimeOld.tv_usec) );
		};
	};
	logTimeOld = logTimeNew;
};

// Diese Datenstruktur repräsentiert einen einzelnen Eintrag in der Log-Datei.
struct log_entry
{
	struct timeval    timeval;
	struct can_frame  can_frame;
};

int main (void)
{
	int entryCounter = 0;
	struct timeval tvLog;
	int error;
	// Socket mit dem angegebenen CAN-Interface öffnen, danach kann
	// mit read() und write() auf das Interface zugegriffen werden.
	int fd_can;
	fd_can = can_open("vcan0");
	if (fd_can == -1)
		perror("CAN Error");
	// Log-Datei öffnen. Hier sind die Daten in Form von struct log_entry
	// Einträgen hinterlegt, die nacheinander mit read() lesbar sind.
	int fd_log;
	fd_log = open(LOG_FILE, O_RDONLY, 0660);
	if (fd_log == -1)
		perror("Log Error");
	// Einzelnen Log-Eintrag auslesen.
	struct log_entry entry;
	int eof = 1;
	while (eof != 0)
	{
		eof = read(fd_log, &entry, sizeof(struct log_entry));
		if (eof == -1){
			perror("Read Error");
		}
		if (eof == 0) {
			printf("End of File\n");
		}

		// Eine einzelne Nachricht an das CAN-Interface senden.
		// Als Inhalt dient das can_frame innerhalb der log_entry Struktur.
		error = write(fd_can, &entry.can_frame, sizeof(struct can_frame));
		if (error == -1)
		{
			perror("Write Error");
			return 0;
		}	
		entryCounter++;	
		suspendAccordingToLogTime(entryCounter, entry.timeval);
		if(entryCounter == 1){
			tvLog = entry.timeval;
		};
	}

	close(fd_log);
	// Programm beenden
	return 0;
}
