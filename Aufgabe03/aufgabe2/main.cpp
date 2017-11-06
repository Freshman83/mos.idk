// --------------------------------------------------------------------------
// DashBoard - Aufgabe 1
//
// Bearbeitet von:
//     Sascha Niklas, Matrikelnummer
//     David Rotärmel, Matrikelnummer
//
// --------------------------------------------------------------------------

// Header
#include <stdio.h>       // printf, fprintf
#include <stdlib.h>      // exit
#include <unistd.h>      // sleep
#include <math.h>        // z.B. M_PI, cos(), sin()
#include <GL/glut.h> 
#include "../lib/gles.h" // struct opengles, gles*-Funktionen
#include "../lib/tile.h" // struct tile, loadPngTile

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

int main(void)
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
	
	GLfloat angle = kmh2deg(30);
	do
	{
		// Framebuffer löschen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Dashboard zeichnen
		tileDraw(&dashboard);


		// ---- Linke Tachonadel zeichnen ---------------------------

		glPushMatrix();

		// Tachonadel verschieben.
		glTranslatef(-1.0,0.0,0.0);

		// Tachonadel rotieren.
		// 135.0° = 0 km/h; 0.0° = 90 km/h => 1,5° = 1 km/h
		glRotatef(angle,0.0,0.0,1.0);

		// Tachonadel verschieben.
		glTranslatef(0.0,0.25,0.0);

		// Tachonadel zeichnen.
		tileDraw(&needle);

		glPopMatrix();

		// ---- Das gezeichnete Bild sichtbar machen ----------------
		glesDraw(&opengles);
		usleep(1000 * 1000);

	}
	while(glesRun(&opengles));

	// OpenGL ES Ressourcen freigeben.
	glesDestroy(&opengles);

	return 0;
}
