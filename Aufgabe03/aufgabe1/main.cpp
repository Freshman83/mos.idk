// --------------------------------------------------------------------------
// DashBoard - Aufgabe 1
//
// Bearbeitet von:
//     Name, Matrikelnummer
//     Name, Matrikelnummer
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

	do
	{
		// Framebuffer löschen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Dashboard zeichnen
		tileDraw(&dashboard);


		// ---- Linke Tachonadel zeichnen ---------------------------

		glPushMatrix();

			// Tachonadel verschieben.
			// ... ?

			// Tachonadel rotieren.
			// ... ?

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
