/* imgfapper.h
 * Created: 26 Dec 2014
 * Faps images
 */

#include <png.h>

#ifndef PNGEN_H
#define PNGEN_H

/* data structure to represent image properties */
struct image_info {
	int width;
	int height;
	size_t row_size; /*size of row array in bytes */
	int bpp; /* bytes per pixel */
	int color; /* bool to denote color vs grayscale */
};

/* data structure to represent pixel */
struct pixel {
	int x; /* horizontal position */
	int y; /* vertical position */
	struct image_info *info; /* image_info struct for containing image */
	png_bytep data; /* pointer to data value(s) */
};


/* seed random number generator (if it hasn't already been) */
void seed_rand();

/* Generate a PNG image using provided pixel generation algorithm (pixgen) */
void gen_png(png_bytep *pixels, struct image_info *info, void (*pixgen) (struct pixel *));

/* Generates random pixel color values */
void pixgen_rand(struct pixel *pixel);

/* Generates a random PNG image */
void gen_png_rand(png_bytep *pixels, struct image_info *info);

/* Generates pixel color values via sinusoidal a function */
void pixgen_sin(struct pixel *pixel);

/* Generates a PNG image w/ sinusoidally-calculated pixels */
void gen_png_sin(png_bytep *pixels, struct image_info *info);

#endif