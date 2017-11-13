// --------------------------------------------------------------------------
// DashBoard - Aufgabe 3.3
//
// Bearbeitet von:
//     Sascha Niklas, 257146
//     David Rotärmel, 258201
//
// --------------------------------------------------------------------------

// Header
#include <stdio.h>       // printf, fprintf
#include <stdlib.h>      // exit
#include <unistd.h>      // sleep
#include <pthread.h>
#include <GL/glut.h> 
#include "draw.h"

int main(void)
{
	pthread_t draw_thread;
	pthread_create(&draw_thread, 0, draw_main, NULL);
	pthread_join(draw_thread, 0);
	return 0;
}
