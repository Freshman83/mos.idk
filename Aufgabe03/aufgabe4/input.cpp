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
	
	for(float i = 0.0f; i <= 1000.0f; i += 100.0f)
	{
		db->kmh = 50.0f + i;
		db->rpm = 3000.0f + i;
		usleep(33 * 1000);
	}

	return 0;
}
