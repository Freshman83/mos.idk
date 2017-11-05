// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

#include <stdio.h>     // fprintf, stderr
#include <stdlib.h>    // exit
#include <GL/glut.h>  
#include <X11/Xlib.h>  // XLib Datentypen und Funktionen
#include "gles.h"

// --------------------------------------------------------------------------
//   Verschiedenes
// --------------------------------------------------------------------------
// Einfache Hilfsfunktion um den Lesefluss des Programms zu verbessern.
// Ist value != 0, wird der Name der fehlgeschlagenen Funktion
// zusammen mit dem dazugehören EGL-Fehlercode ausgegeben.
// Genauere Informationen über die EGL-Fehlercodes finden sich hier:
//   www.khronos.org/registry/egl/sdk/docs/man/xhtml/eglGetError.html
static inline void handleEglError(const int value, const char *name)
{
	if (value == 0) { return; }
	fprintf(stderr,	"%s fehlgeschlagen!\n", name);
	fprintf(stderr, "EGL Fehler 0x%x\n", eglGetError());
	exit(-1);
}

// --------------------------------------------------------------------------
//   OpenGL ES Kontext samt Oberfläche zum Zeichnen erstellen
// --------------------------------------------------------------------------
int glesInitialize(struct opengles *opengles)
{
	// ------------------------------------------------------------------
	//   XLib
	// ------------------------------------------------------------------

	// Handler für eine Verbindung zu einem X-Server.
	Display *xlib_display;

	// Handler für ein Fenster innerhalb eines X-Servers.
	Window xlib_window;

	// Handler für das Fenster, in dem unser eigenes Fenster erstellt wird.
	Window xlib_parent;

	// Attribute für das zu erstellende Fenster.
	const uint32_t  x = 0,
	                y = 0,
	            width = 1024,
	           height = 512,
	     border_width = 0,
	           border = 0,
	       background = 0;

	// ---- Initialisierung ---------------------------------------------

	// Verbindung zu Standard-X-Server (localhost) aufbauen.
	xlib_display = XOpenDisplay(0);
	if (xlib_display == 0) {
		fprintf(stderr, "XOpenDisplay fehlgeschlagen!\n");
		exit(-1);
	}

	// Handler für Hauptfenster von xlib_display auslesen.
	xlib_parent = XDefaultRootWindow(xlib_display);

	// Neues X-Fenster erstellen
	xlib_window = XCreateSimpleWindow(xlib_display, xlib_parent,
	                                  x, y, width, height,
	                                  border_width, border, background);

	// Dem Fenster einen Namen geben.
	XStoreName(xlib_display, xlib_window, "swe3-dashboard");

	// Das Fenster sichtbar machen.
	XMapWindow(xlib_display, xlib_window);

	// Alle Befehle sofort an den X-Server senden.
	XFlush(xlib_display);

	Atom wmDeleteMessage;
	wmDeleteMessage = XInternAtom(xlib_display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(xlib_display, xlib_window, &wmDeleteMessage, 1);

	// ------------------------------------------------------------------
	//   EGL
	// ------------------------------------------------------------------

	// Zwischenspeicher für Rückgabewerte von Funktionen wie eglInitialize.
	EGLBoolean ret;

	// Zwei Integer die die Major und Minor Version von EGL beinhalten.
	EGLint eglmajor, eglminor;

	// Handler für ein Display, hier eine Verbindung zu einem X-Server.
	EGLDisplay display;

	// Handler für ein Surface, in diesem Fall dem Framebuffer.
	EGLSurface surface;

	// Liste mit Eigenschaften die der Framebuffer haben muss.
	const EGLint configAttr[] = {
		EGL_SURFACE_TYPE    , EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE , EGL_OPENGL_ES_BIT,
		EGL_NONE
	};

	// Rückgabewerte von eglChooseConfig. Beinhalten eine (oder mehrere)
	// Konfiguration(en) mit den von configAttr geforderten Eigenschaften.
	EGLConfig   config;
	EGLint      configCount;

	// Handler für einen OpenGL-Kontext.
	EGLContext  context;

	// Version des OpenGL ES Kontextes festlegen.
	const EGLint contextAttr[] = {
		EGL_CONTEXT_CLIENT_VERSION , 1,
		EGL_NONE
	};

	// ---- Initialisierung ---------------------------------------------

	// EGL-Display-Handler für unsere im oberen Abschnitt
	// aufgebaute X-Server-Verbindung erstellen.
	display = eglGetDisplay(xlib_display);
	handleEglError(display == EGL_NO_DISPLAY, "eglGetDisplay");

	// Eine Verbindung zwischen EGL und X-Server herstellen
	// sowie die EGL Version auslesen und ausgeben
	ret = eglInitialize(display, &eglmajor, &eglminor);
	handleEglError(ret == EGL_FALSE, "eglInitialize");
	printf("EGL Version %d.%d\n", eglmajor, eglminor);

	// Legt die zu von EGL zu verwendende Render-API fest, hier OpenGL ES.
	ret = eglBindAPI(EGL_OPENGL_ES_API);
	handleEglError(ret == EGL_FALSE, "eglBindAPI");

	// Überprüfen ob uns display einen Framebuffer mit den Eigenschaften
	// aus configAttr zur Verfügung stellen kann.
	// Anzahl der möglichen Rückgabewerte dabei auf 1 limitieren.
	ret = eglChooseConfig(display, configAttr, &config, 1, &configCount);
	handleEglError(ret == EGL_FALSE, "eglChooseConfig");
	if (configCount == 0)
	{
		fprintf(stderr, "eglChooseConfig fehlgeschlagen!\n");
		fprintf(stderr, "configCount == 0, configAttr anpassen!\n");
		exit(-1);
	}

	// Ein surface, hier gleichzeitig der Framebuffer, erstellen und
	// mit dem im oberen Abschnitt erstellten X-Fenster verknüpfen.
	surface = eglCreateWindowSurface(display, config, xlib_window, 0);
	handleEglError(surface == EGL_NO_SURFACE, "eglCreateWindowSurface");

	// OpenGL-Kontext erstellen
	context = eglCreateContext(display, config, 0, contextAttr);
	handleEglError(context == EGL_NO_CONTEXT, "eglCreateContext");

	// Display, X-Fenster-Framebuffer und OpenGL-Kontext miteinander
	// verknüpfen und als Basis für alle folgenden gl...-Befehle verwenden
	ret = eglMakeCurrent(display, surface, surface, context);
	handleEglError(ret == EGL_FALSE, "eglMakeCurrent");

	// Variablen für OpenGLES-Struktur kopieren
	opengles->display = display;
	opengles->surface = surface;
	opengles->context = context;
	opengles->xlib_display = xlib_display;
	opengles->xlib_window = xlib_window;
	opengles->width = (uint16_t)width;
	opengles->height = (uint16_t)height;
	opengles->ratio = (GLfloat)width / (GLfloat)height;

	// Unterstützung für Texturen aktivieren.
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0,0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef((GLfloat)height / (GLfloat)width, 1.0f, 1.0f);
	//glOrthof(-1,1,-1,1,-1,1);

	// Hintergrundfarbe nach löschen des Framebuffer festlegen.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return 0;
}

// --------------------------------------------------------------------------
//   OpenGL ES Buffer austauschen
// --------------------------------------------------------------------------
int glesDraw(struct opengles *opengles)
{
	// Das gezeichnete Bild sichtbar machen
	eglSwapBuffers(opengles->display, opengles->surface);




	return 0;
}

// --------------------------------------------------------------------------
//   OpenGL ES Buffer austauschen
// --------------------------------------------------------------------------
int glesRun(struct opengles *gles)
{
	XEvent event;

	// Alle Events abarbeiten.
	while(XPending(gles->xlib_display))
	{
		// Nächstes Event auslesen.
		XNextEvent(gles->xlib_display, &event);

		// Mini-Handler um WM_DELETE_WINDOW abzufangen.
		switch(event.type)
		{
		case ClientMessage: return 0; break;
		default: break;
		}
	}

	return 1;
}


// --------------------------------------------------------------------------
//   Ressourcen freigeben
// --------------------------------------------------------------------------
int glesDestroy(struct opengles *opengles)
{
	// ---- EGL ---------------------------------------------------------

	// Kontext deaktivieren
	eglMakeCurrent(opengles->display, 0, 0, 0);

	// Speicher für Surface freigeben
	eglDestroySurface(opengles->display, opengles->surface);

	// Speicher für Kontext freigeben
	eglDestroyContext(opengles->display, opengles->context);

	// Verbindung zu display schließen
	eglTerminate(opengles->display);

	// EGL-Backend vollständig zurücksetzen
	eglReleaseThread();

	// ---- XLib --------------------------------------------------------

	// Speicher für Fenster freigeben
	XDestroyWindow(opengles->xlib_display, opengles->xlib_window);

	// Verbindung zu X-Server schließen
	XCloseDisplay(opengles->xlib_display);

	return 0;
}


