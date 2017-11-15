// --------------------------------------------------------------------------
// DashBoard - Aufgabe 4
//
// Bearbeitet von:
//     Sascha Niklas, 257146
//     David Rotärmel, 258201
//
// --------------------------------------------------------------------------

// Header
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <GL/glut.h> 
#include "draw.h"
#include "input.h"

int main(void)
{
	GLfloat kmh = 70.0;

	pthread_t input_thread;
	pthread_create(&input_thread, 0, input_main, NULL);
	pthread_t draw_thread;
	pthread_create(&draw_thread, 0, draw_main, (void*)&kmh);
	pthread_join(draw_thread, 0);
	return 0;
}
