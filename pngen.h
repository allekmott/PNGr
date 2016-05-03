/* pngen.h
 * Created: 26 Dec 2014
 * Generates images
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


/* take a value b/w -1 and 1, extend to -256 to 255 */
png_byte nice_colorval(float v);


/***************************************************************
 * This is where shit starts getting a bit crazy               *
 ***************************************************************/
/* Simple 2-var function, used to test composition functions */
float lol_func(float s, float t);

/* Recursively composes func(s, t) together, creating a compound
 * phase modulation effecty thing
 */
float comp_func_phase(float (*func) (float, float), float s, float t, int levels);

/* Recursively composes sines together, so as to achieve
 * strange phasing effect
 */
float comp_sine_phase(float s, float t, int levels);

/* Recursively composes sines together, so as to achieve
 * frequency modulating effect
 */
float comp_sine_freq(float s, float t, int levels);

/* Recursively composes sines toether, so as to achieve
 * amplitude modulating effect
 */
float comp_sine_amp(float s, float t, int levels);

#endif