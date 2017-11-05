// --------------------------------------------------------------------------
//  Copyright © 2014  CS -  All rights reserved
// --------------------------------------------------------------------------

#include <stdio.h>     // fopen, fread, fprintf, perror
#include <stdlib.h>
#include <png.h>       // PNG Datentypen und Funktionen
#include <GL/glut.h> 
#include "gles.h"      // struct opengles, gles*-Funktionen
#include "tile.h"      // struct tile, loadPngTile

// --------------------------------------------------------------------------
//   PNG als sofort nutzbares OpenGL ES Objekt laden.
// --------------------------------------------------------------------------
int tileLoadPng(struct opengles *gles, struct tile *tile, const char *file)
{
	// Zwischenspeicher für Rückgabewerte
	int ret;

	// PNG-Datei im Binärformat öffnen.
	FILE *png_file = fopen(file, "rb");
	if (png_file == 0) {
		perror("loadPngTile.fopen");
		exit(-1);
	}

	// PNG-Header auslesen.
	png_byte png_header[8];
	ret = fread(png_header, 1, 8, png_file);
	if (ret < 0) {
		perror("loadPngTile.fread");
		exit(-1);
	}

	// Handelt es sich um eine PNG-Datei?
	ret = png_sig_cmp(png_header, 0, 8);
	if (ret) {
		fprintf(stderr, "%s ist keine PNG-Datei!\n", file);
		exit(-1);
	}

	// PNG-Struktur für Zugriffe anlegen.
	png_structp png_read_ptr;
	png_read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if (!png_read_ptr) {
		fprintf(stderr, "png_create_read_struct fehlgeschlagen!\n");
		fclose(png_file);
		exit(-1);
	}

	// PNG-Strukturen für Informationen über das Bild anlegen
	png_infop png_info_ptr, png_iend_ptr;
	png_info_ptr = png_create_info_struct(png_read_ptr);
	png_iend_ptr = png_create_info_struct(png_read_ptr);
	if (!png_info_ptr || !png_iend_ptr)
	{
		fprintf(stderr, "png_create_info_struct fehlgeschlagen!\n");
		png_destroy_read_struct(&png_read_ptr, 0, 0);
		fclose(png_file);
		exit(-1);
	}

	// Fehlerbehandlung für weitere PNG-Zugriffe initialisieren
	ret = setjmp(png_jmpbuf(png_read_ptr));
	if (ret) {
		fprintf(stderr, "png_jmpbuf fehlgeschlagen!\n");
		png_destroy_read_struct(&png_read_ptr, &png_info_ptr,
		                        &png_iend_ptr);
		fclose(png_file);
		exit(-1);
	}

	// IO-Zugriff auf das PNG initialisieren.
	png_init_io(png_read_ptr, png_file);

	// Signalisieren dass die ersten 8 Bytes der Datei gelesen wurden.
	png_set_sig_bytes(png_read_ptr, 8);

	// PNG-Header auslesen.
	png_read_info(png_read_ptr, png_info_ptr);

	// Bild-Informationen aus png_info-Struktur auslesen
	png_uint_32 width, height;
	int depth, type;
	png_get_IHDR(png_read_ptr, png_info_ptr,
	             &width, &height, &depth, &type, 0, 0, 0);

	// Anzahl der Bytes einer Zeile auslesen.
	int rowsize;
	rowsize = png_get_rowbytes(png_read_ptr, png_info_ptr);

	// Speicherbereich für die Rohdaten des Bilds anlegen.
	png_byte *png_data;
	png_data = (png_byte*)malloc(rowsize * height);
	if (!png_data) {
		fprintf(stderr, "malloc fehlgeschlagen!\n");
		png_destroy_read_struct(&png_read_ptr, &png_info_ptr,
		                        &png_iend_ptr);
		fclose(png_file);
		exit(-1);
	}

	// Pointer-Array für die Zeilen des Bildes anlegen.
	png_bytep *png_row_ptrs;
	png_row_ptrs = (png_bytep*)malloc(sizeof(png_bytep) * height);
	if (!png_data) {
		fprintf(stderr, "malloc fehlgeschlagen!\n");
		png_destroy_read_struct(&png_read_ptr, &png_info_ptr,
		                        &png_iend_ptr);
		fclose(png_file);
		free(png_data);
		exit(-1);
	}

	// Pointer-Array setzen. Jeder Eintrag zeigt dabei auf den Anfang
	// einer einzelnen Zeile innerhalb des neu angelegten Speicherbereichs.
	png_uint_32 i;
	for (i = 0; i < height; i++)
	{
		png_row_ptrs[height - 1 - i] = png_data + i * rowsize;
	}

	// Das Bild mithilfe des Pointer-Arrays auslesen.
	png_read_image(png_read_ptr, png_row_ptrs);

	// Vertex-Koordinaten berechnen.
	GLfloat x_coord = (gles->ratio) / (gles->width  / (GLfloat)width );
	GLfloat y_coord = (       1.0f) / (gles->height / (GLfloat)height);

	// Vertex-Koordinaten setzen.
	tile->vertices[0] = -x_coord;
	tile->vertices[2] = -x_coord;
	tile->vertices[4] =  x_coord;
	tile->vertices[6] =  x_coord;
	tile->vertices[1] = -y_coord;
	tile->vertices[3] =  y_coord;
	tile->vertices[5] = -y_coord;
	tile->vertices[7] =  y_coord;

	// Textur-Objekt anlegen, falls nötig.
	if (tile->texture == 0)
	{
		glGenTextures(1, &(tile->texture));
	}

	// Textur-Objekt "in den Vordergrund holen".
	glBindTexture(GL_TEXTURE_2D, tile->texture);

	// Textur-Objekt mit dem aus dem PNG ausgelesenen Daten verknüpfen.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
	             GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)png_data);

	// Filter-Mechanismen für das Textur-Objekt festlegen.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Ressourcen von libpng freigeben.
	png_destroy_read_struct(&png_read_ptr, &png_info_ptr, &png_iend_ptr);
	free(png_data);
	free(png_row_ptrs);
	fclose(png_file);

	return 0;
}


// --------------------------------------------------------------------------
//   Vorhandenes Tile zeichnen.
// --------------------------------------------------------------------------
int tileDraw(struct tile *tile)
{
	// OpenGL mitteilen, dass Vertex- und Texturdaten geschrieben werden.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Dashboard-Textur verwenden.
	glBindTexture(GL_TEXTURE_2D, tile->texture);

	// Zu verwendende Textur-Koordinaten festlegen.
	glTexCoordPointer(2, GL_FLOAT, 0, tile_texture_coords);

	// Dashboard-Vertex-Koordinaten übergeben.
	glVertexPointer(2, GL_FLOAT, 0, tile->vertices);

	// Objekt zeichnen.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Vertex- und Texturdaten wurden geschrieben.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	return 0;
}

