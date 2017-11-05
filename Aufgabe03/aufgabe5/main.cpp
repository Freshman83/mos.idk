// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

#include <stdio.h>     // printf, fprintf
#include <stdlib.h>    // exit
#include <unistd.h>    // sleep
#include <pthread.h>
#include <GL/glut.h> 

#include "dashboard.h"
#include "input.h"
#include "draw.h"

int main(int argc, char **argv)
{
	dashboard_t *db = new dashboard_t();

	db->fuel = 0.5f;

	     if (argc > 2) { db->dev = argv[1]; db->type = argv[2]; }
	else if (argc > 1) { db->dev = argv[1]; db->type = "mki"; }
	else               { db->dev = "vcan0"; db->type = "mki"; }

	// Create worker threads
	pthread_t draw_thread;
	pthread_t input_thread;
	pthread_create(&draw_thread,  0, draw_main,  (void*)db);
	pthread_create(&input_thread, 0, input_main, (void*)db);

	pthread_join(draw_thread, 0);

	return 0;
}
