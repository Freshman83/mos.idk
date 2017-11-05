// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

#ifndef _GNU_SOURCE
#define _GNU_SOURCE    // Nötig für asprintf()
#endif

#include <stdio.h>     // asprintf()
#include <stdlib.h>    // abort()
#include <unistd.h>    // usleep
#include <math.h>      // tanh
#include <GL/glut.h> 

#include "dashboard.h"
#include "gles.h"
#include "tile.h"
#include "draw.h"

// --------------------------------------------------------------------------
//  Hilfsfunktionen
// --------------------------------------------------------------------------
GLfloat kmh2deg(GLfloat kmh)
{
	// Wertebereich einschränken
	     if (kmh > 150.0f) { kmh = 150.0f; }
	else if (kmh <   0.0f) { kmh =   0.0f; }

	// Der Tacho besitzt 16 Markierungen im Abstand von 15 Grad.
	kmh *= 1.5f;

	// Offset für Nullstellung hinzufügen.
	kmh = 135.0f - kmh;

	return kmh;
}

GLfloat rpm2deg(GLfloat rpm)
{

	// Wertebereich einschränken
	     if (rpm > 7000.0f) { rpm = 7000.0f; }
	else if (rpm <    0.0f) { rpm =    0.0f; }

	// Der Tacho besitzt 16 Markierungen im Abstand von 15 Grad.
	rpm *= 0.03f;

	// Offset für Nullstellung hinzufügen.
	rpm += -135.0f;

	return rpm;
}

GLfloat temp2deg(GLfloat temp)
{

	// Wertebereich einschränken
	     if (temp > 120.0f) { temp = 120.0f; }
	else if (temp <  40.0f) { temp =  40.0f; }

	temp -= 40.0f;

	// Der Tacho besitzt 9 Markierungen im Abstand von 13.33 Grad.
	temp *= (9.0f * 13.33f) / 80.0f;

	// Offset für Nullstellung hinzufügen.
	temp = 150.0f - temp;

	return temp;
}

GLfloat fuel2deg(GLfloat val)
{

	// Wertebereich einschränken
	     if (val > 1.0f) { val = 1.0f; }
	else if (val < 0.0f) { val = 0.0f; }

	// Der Tacho besitzt 9 Markierungen im Abstand von 13.33 Grad.
	val *= (9.0f * 13.33f);

	// Offset für Nullstellung hinzufügen.
	val += -150.0f;

	return val;
}

GLfloat filter(GLfloat value, GLfloat *buffer)
{
	//EC *buffer = *buffer + tanh((value - *buffer) / 16.0f);
	*buffer= value;
	return *buffer;
}


// --------------------------------------------------------------------------
//  Oberfläche in Endlosschleife zeichnen
// --------------------------------------------------------------------------
void* draw_main(void *data)
{
	int ret;
	struct dashboard *db = (struct dashboard *)data;

	// OpenGL ES initialisieren
	struct opengles opengles;
	glesInitialize(&opengles);

	char *title = 0;
	ret = asprintf(&title, "dashboard-demo - %s - %s ", \
	                        db->dev, db->type);
	if (!ret) { abort(); }

	// Fenster Umbenennen
	XStoreName(opengles.xlib_display, opengles.xlib_window, title);

	// Textur für Dashboard laden
	struct tile dashboard = TILE_ZEROINIT;
	tileLoadPng(&opengles, &dashboard, "../bilder/dashboard_full.png");

	// Textur für Tachonadel laden
	struct tile needle = TILE_ZEROINIT;
	tileLoadPng(&opengles, &needle, "../bilder/needle.png");

	// Textur für Blinker Links laden
	struct tile blink = TILE_ZEROINIT;
	tileLoadPng(&opengles, &blink, "../bilder/blinker.png");

	GLuint flags = 0;
	GLuint counter = 0;
	GLfloat rpmdeg = 0.0;
	GLfloat kmhdeg = 0.0;
	GLfloat tempdeg = 0.0;
	GLfloat fueldeg = 0.0;
	do
	{
		kmhdeg = filter(kmh2deg(db->kmh), &kmhdeg);
		rpmdeg = filter(rpm2deg(db->rpm), &rpmdeg);
		tempdeg = filter(temp2deg(db->temp), &tempdeg);
		fueldeg = filter(fuel2deg(db->fuel), &fueldeg);

		// Blinker Logik
		if (db->lamps & DASHBOARD_LEFT) {
			if (!(counter % 25)) {
				flags ^= DASHBOARD_LEFT;
			}
		}
		else {
			flags &= ~DASHBOARD_LEFT;
		}

		if (db->lamps & DASHBOARD_RIGHT) {
			if (!(counter % 25)) {
				flags ^= DASHBOARD_RIGHT;
			}
		}
		else {
			flags &= ~DASHBOARD_RIGHT;
		}


		// Framebuffer löschen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Dashboard zeichnen
		tileDraw(&dashboard);

		// ---- Linke Tachonadel zeichnen ---------------------------

		// Aktuelle Transformationsmatrix sichern.
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

		// ---- Benzin-Nadel zeichnen --------------------------
		glPushMatrix();

			// Tachonadel an ihre richtige Position schieben.
			glTranslatef(1.0f/8.0f, -0.564f, 0);

			// Nadel skalieren
			glScalef(0.5f, 0.5f, 0);

			// Tachonadel rotieren
			glRotatef(fueldeg, 0, 0, 1);

			// Rotationszentrum der Tachonadel korrigieren.
			glTranslatef(0, 0.25f, 0);

			// Tachonadel zeichnen.
			tileDraw(&needle);

		glPopMatrix();

		// ---- Temperatur-Nadel zeichnen --------------------------
		glPushMatrix();

			// Tachonadel an ihre richtige Position schieben.
			glTranslatef(-1.0f/8.0f, -0.564f, 0);

			// Nadel skalieren
			glScalef(0.5f, 0.5f, 0);

			// Tachonadel rotieren
			glRotatef(tempdeg, 0, 0, 1);

			// Rotationszentrum der Tachonadel korrigieren.
			glTranslatef(0, 0.25f, 0);

			// Tachonadel zeichnen.
			tileDraw(&needle);

		glPopMatrix();

		// ---- Blinker Links zeichnen --------------------------
		if (flags & DASHBOARD_LEFT)
		{
			glPushMatrix();
				glTranslatef(-5.0f/8.0f, -28.0f/32.0f, 0);
				tileDraw(&blink);
			glPopMatrix();
		}

		// ---- Blinker Rechts zeichnen --------------------------
		if (flags & DASHBOARD_RIGHT)
		{
			glPushMatrix();
				glTranslatef(5.0f/8.0f, -28.0f/32.0f, 0);
				glScalef(-1.0f, 1.0f, 1.0f);
				tileDraw(&blink);
			glPopMatrix();
		}

		// Das gezeichnete Bild sichtbar machen.
		glesDraw(&opengles);

		// 12ms warten um CPU-Last zu verringern.
		usleep(20 * 1000);

		// Counter inkrementieren.
		counter++;
	}
	while(glesRun(&opengles));

	// OpenGL ES Ressourcen freigeben.
	glesDestroy(&opengles);

	return 0;
}
