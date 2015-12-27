/* imgfapper.h
 * Created: 26 Dec 2014
 * Faps images
 */

#include <png.h>

#ifndef IMGFAPPER_H
#define IMGFAPPER_H

/* Generates a random PNG image w/ provided dimensions */
void fap_png(png_bytep *pixels, size_t row_size, int width, int height);

#endif