// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

// Sicherstellen, dass die Header-Datei nur einmal eingebunden wird.
#ifndef GLES_H_
#define GLES_H_ 1

#include <EGL/egl.h>   // EGL Datentypen und Funktionen

struct opengles
{
	// EGL
	EGLDisplay  display;
	EGLSurface  surface;
	EGLContext  context;

	// Xlib
	Display    *xlib_display;
	Window      xlib_window;

	// Dimensionen des Fensters
	uint16_t    width;
	uint16_t    height;
	float       ratio;
};

// Funktionsprototyp für can_open-Funktion.
int glesInitialize(struct opengles *opengles);
int glesDraw(struct opengles *opengles);
int glesRun(struct opengles *gles);
int glesDestroy(struct opengles *opengles);

#endif
