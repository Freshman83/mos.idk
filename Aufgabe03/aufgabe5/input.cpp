// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

#include <stdio.h>      // printf(), perror()
#include <stdint.h>     // uint32_t, ...
#include <stdlib.h>     // exit()
#include <string.h>     // strcmp()
#include <unistd.h>     // write()
#include <errno.h>      // errno
#include <math.h>       // M_PI
#include <fcntl.h>      // O_WRONLY, O_CREAT
#include <net/if.h>     // struct ifreq
#include <sys/ioctl.h>  // ioctl(), SIOCGIFINDEX
#include <GL/glut.h> 

#include "can.h"
#include "input.h"


// --------------------------------------------------------------------------
//  Hauptschleife zum Einlesen von CAN-Daten.
// --------------------------------------------------------------------------
void* input_main(void *data)
{
	dashboard_t *db = (dashboard_t *)data;

	// CAN-Interface öffnen
	int fd_can;
	fd_can = can_open(db->dev);

	// Pointer auf Funktion anlegen
	int (*can_parse)(struct can_frame *, dashboard_t *) = 0;

	     if (!strcmp(db->type, "mki")  ) { can_parse = can_parse_mki;   }
	else if (!strcmp(db->type, "astra")) { can_parse = can_parse_astra; }
	else if (!strcmp(db->type, "bmw"))   { can_parse = can_parse_bmw;   }

	struct can_frame frame;
	do
	{
		// In das geöffnete Can-Interface schreiben.
		int ret;
		ret = read(fd_can, &frame, sizeof(struct can_frame));
		if (ret < 0) {
			perror("can_main.read");
			exit(-1);
		}

		// Eingelesenen Can-Frame auswerten.
		can_parse(&frame, db);
	}
	while(1);

	return 0;
}


// --------------------------------------------------------------------------
//  Auswerten von MKI-Can-Frames
// --------------------------------------------------------------------------
int can_parse_mki(struct can_frame *frame, dashboard_t *db)
{
	uint32_t id;
	id = frame->can_id & CAN_EFF_MASK;

	switch(id)
	{
	case 0x200:
		db->kmh  = (frame->data[2] + frame->data[4]) * 8.0f;
		db->kmh += (frame->data[3] + frame->data[5]) / 51.0f;
		break;

	case 0x308:
		db->rpm = ((float)frame->data[1] * 250) + frame->data[2];
		break;

	case 0x608:
		db->temp = (float)frame->data[0] - 40.0f;
		break;

	default:
		break;
	}

	return 0;
}


// --------------------------------------------------------------------------
//  Auswerten von Open Astra Can-Frames
// --------------------------------------------------------------------------
int can_parse_astra(struct can_frame *frame, dashboard_t *db)
{
	uint32_t id;
	id = frame->can_id & CAN_EFF_MASK;

	uint8_t *d = &(frame->data[0]);

	switch(id)
	{
	case 0x110:
		db->rpm = ((uint16_t)(d[2] | (d[1] << 8)));
		printf("%4x %d\n", (uint16_t)(d[2] | (d[1] << 8)), (uint16_t)(d[2] | (d[1] << 8)));
		break;

	case 0x280:
		db->kmh =
			((uint16_t)(d[1] + ((d[0] & 0x3F) << 8)) * 0.25f) +
			((uint16_t)(d[3] + ((d[2] & 0x3F) << 8)) * 0.25f) +
			((uint16_t)(d[5] + ((d[4] & 0x3F) << 8)) * 0.25f) +
			((uint16_t)(d[7] + ((d[6] & 0x3F) << 8)) * 0.25f);

		db->kmh /= 4.0f;
		db->kmh /= 60.0f;
		db->kmh *= 13.0f * 2.54f * M_PI * 3.6f;

		break;

	default:
		break;
	}

	return 0;
}


// --------------------------------------------------------------------------
//  Auswerten von BMW Can-Frames
// --------------------------------------------------------------------------
int can_parse_bmw(struct can_frame *frame, dashboard_t *db)
{
	uint32_t id;
	id = frame->can_id & CAN_EFF_MASK;

	uint8_t *d = &(frame->data[0]);

	switch(id)
	{
	case 0x254:
		db->kmh =
			( (((d[1]*256.0f + d[0]) - 32767.0f) / 50.0f) +
			  (((d[3]*256.0f + d[2]) - 32767.0f) / 50.0f) +
			  (((d[5]*256.0f + d[4]) - 32767.0f) / 50.0f) +
			  (((d[7]*256.0f + d[6]) - 32767.0f) / 50.0f) ) / 4.0f;
		break;

	case 0xa5:
		db->rpm = ((d[6]* 256.0f) + d[5]) / 4.0f;
		break;

	case 0x1f6:
		db->lamps &= ~(DASHBOARD_LEFT | DASHBOARD_RIGHT);
		     if (d[0] == 145) { db->lamps |= DASHBOARD_LEFT;  }
		else if (d[0] == 161) { db->lamps |= DASHBOARD_RIGHT; }
		break;

	default:
		break;
	}

	return 0;
}
