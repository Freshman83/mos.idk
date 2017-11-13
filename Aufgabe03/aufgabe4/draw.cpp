// --------------------------------------------------------------------------
// DashBoard - Aufgabe 4
//
// Bearbeitet von:
//     Sascha Niklas, 257146
//     David Rotärmel, 258201
//
// --------------------------------------------------------------------------

#ifndef _GNU_SOURCE
#define _GNU_SOURCE    
#endif

#include <stdio.h>     
#include <stdlib.h>     
#include <unistd.h>      
#include <math.h>       
#include <GL/glut.h> 
#include "gles.h" 
#include "tile.h" 
#include "draw.h"

GLfloat kmh2deg(GLfloat kmh)
{
	if (0.0f < kmh && kmh <= 150.0f)
	{
		return 135.0f - kmh * 1.5f;
	}
	else if (kmh > 150.0f)
	{
		return 150.0f;
	}
	else
	{
		return 0.0;
	}
}

GLfloat rpm2deg(GLfloat rpm)
{
	if (0.0f < rpm && rpm <= 7000.0f)
	{
		return -135.0f + rpm * 0.03f;
	}
	else if (rpm > 7000.0f)
	{
		return 7000.0f;
	}
	else
	{
		return 0.0;
	}
}

int draw_main(void)
{
	// OpenGL ES initialisieren
	struct opengles opengles;
	glesInitialize(&opengles);

	// Textur für Dashboard laden
	struct tile dashboard = TILE_ZEROINIT;
	tileLoadPng(&opengles, &dashboard, "../bilder/dashboard.png");

	// Textur für Tachonadel laden
	struct tile needle = TILE_ZEROINIT;
	tileLoadPng(&opengles, &needle, "../bilder/needle.png");

	GLuint counter = 0;
	GLfloat rpmdeg = rpm2deg(0.0);
	GLfloat kmhdeg = kmh2deg(0.0);
	do
	{
		// Framebuffer löschen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Dashboard zeichnen
		tileDraw(&dashboard);

		// ---- Linke Tachonadel zeichnen ---------------------------
		glPushMatrix();
		// Tachonadel an ihre richtige Position schieben.
		glTranslatef(-1.0f, 0, 0);
		// Tachonadel rotieren
		glRotatef(kmhdeg, 0, 0, 1);
		// Rotationszentrum der Tachonadel korrigieren.
		glTranslatef(0, 0.25f, 0);
		// Tachonadel zeichnen.
		tileDraw(&needle);
		// Transformationsmatrix wiederherstellen.
		glPopMatrix();

		// ---- Rechte Tachonadel zeichnen --------------------------
		glPushMatrix();
		// Tachonadel an ihre richtige Position schieben.
		glTranslatef(1.0f, 0, 0);
		// Tachonadel rotieren
		glRotatef(rpmdeg, 0, 0, 1);
		// Rotationszentrum der Tachonadel korrigieren.
		glTranslatef(0, 0.25f, 0);
		// Tachonadel zeichnen.
		tileDraw(&needle);
		glPopMatrix();

		// Das gezeichnete Bild sichtbar machen.
		glesDraw(&opengles);

		usleep(200 * 1000);

		// Counter inkrementieren.
		counter++;
	}
	while(glesRun(&opengles));

	// OpenGL ES Ressourcen freigeben.
	glesDestroy(&opengles);

	return 0;
}
