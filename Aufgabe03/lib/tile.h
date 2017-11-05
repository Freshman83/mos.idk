// --------------------------------------------------------------------------
//  Copyright © 2014  CS  -  All rights reserved
// --------------------------------------------------------------------------

// Sicherstellen, dass die Header-Datei nur einmal eingebunden wird.
#ifndef TILE_H_
#define TILE_H_ 1

struct tile
{
	GLfloat vertices[8];
	GLuint  texture;
};

#define TILE_ZEROINIT { { 0 }, 0 }

// Koordinaten für Texturen festlegen. Da wir in dieser Aufgabe nur
// mit Quadraten arbeiten, können wir die gleichen Textur-Koordinaten
// für alle unsere Texturen nutzen.
static const GLfloat tile_texture_coords[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
};

// PNG als sofort nutzbares OpenGL ES Objekt laden.
int tileLoadPng(struct opengles *gles, struct tile *tile, const char *file);

// Tile zeichnen.
int tileDraw(struct tile *tile);

#endif
