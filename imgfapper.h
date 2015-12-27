/* imgfapper.h
 * Created: 26 Dec 2014
 * Faps images
 */

#include <png.h>

#ifndef IMGFAPPER_H
#define IMGFAPPER_H

/* data structure to represent pixel */
struct pixel {
	int x; /* horizontal position */
	int y; /* vertical position */
	int bytes; /* number of bytes */
	png_bytep data; /* pointer to data value(s) */
};


/* seed random number generator (if it hasn't already been) */
void fap_rand();

/* Generate a PNG image using provided pixel generation algorithm (pixel_fapper) */
void fap_png(png_bytep *pixels, size_t row_size, int width, int height, void (*pixel_fapper) (struct pixel *));

/* Generates random pixel color values */
void pixel_fapper_rand(struct pixel *pixel);

/* Generates a random PNG image */
void fap_png_rand(png_bytep *pixels, size_t row_size, int width, int height);


#endif