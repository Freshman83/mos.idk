// --------------------------------------------------------------------------
// DashBoard - Aufgabe 4
//
// Bearbeitet von:
//     Sascha Niklas, 257146
//     David Rotärmel, 258201
//
// --------------------------------------------------------------------------


#include <stdio.h>      // printf(), perror()
#include <stdint.h>     // uint32_t, ...
#include <unistd.h> 
#include "input.h"

void* input_main(void *data)
{
	dashboard_t *db = (dashboard_t *)data;
	
	for(float i = 0.0f; i <= 7000.0f; i += 100.0f)
	{
		db->kmh = 0.0f + i/100;
		db->rpm = 0.0f + i;
		usleep(100 * 1000);
	}

	return 0;
}
