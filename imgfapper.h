/* imgfapper.h
 * Created: 26 Dec 2014
 * Faps images
 */

#include <png.h>

#ifndef IMGFAPPER_H
#define IMGFAPPER_H

/* Data structure to represent pixel inside of image */
struct pixel {
	int x; /* x location */
	int y; /* y location */
	int bytedepth; /* bytes per pixel */
	png_bytep data; /* data segment */
};

/* Seeds random number generator (if it hasn't been already) */
void fap_rand();

/* Generates random color values for the pixel at the provided location */
void pixel_fapper_rand(struct pixel *pixel);

/* Generates a PNG image with provided dimensions, executing the provided
 * algorithm (pixel_fapper) to generate each pixel
 */
void fap_png(png_bytep *pixels, size_t row_size, int width, int height, void (*pixel_fapper) (struct pixel *));

/* Generates a random PNG image w/ provided dimensions */
void fap_png_rand(png_bytep *pixels, size_t row_size, int width, int height);

#endif