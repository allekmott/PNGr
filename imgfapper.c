/* imgfapper.c
 * Created: 26 Dec 2014
 */

#include <png.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "imgfapper.h"

void fap_rand() {
	static int seeded_rand = 0;
	if (!seeded_rand++)
		srand(time(NULL));
}

void fap_png(png_bytep *pixels, size_t row_size, int width, int height, void (*pixel_fapper) (struct pixel *)) {
	/* to create image of width and height:
	 * height rows of width pixels
	 * row_size / width = bitdepth
	 */

	int bytes_per_pixel = row_size / width;

	int rown;

	/* for each row */
	for (rown = 0; rown < height; rown++) {
		pixels[rown] = malloc(row_size);
		png_bytep row = pixels[rown];

		int pixeln;

		/* for each pixel in the row */
		for (pixeln = 0; pixeln < width; pixeln++) {
			/* create pixel struct */
			struct pixel pixel;
			pixel->x = pixeln;
			pixel->y = rown;
			pixel->bytes = bytes_per_pixel;
			pixel->data = row[pixeln];

			/* fap dat pixel! */
			pixel_fapper(&pixel);
		}
	}
}

void pixel_fapper_rand(struct pixel *pixel) {
	int byten;
	for (byten = 0; byten < pixel->bytes; byten++)
		pixel->data[byten] = (png_byte) (rand() % 256);
}

void fap_png_rand(png_bytep *pixels, size_t row_size, int width, int height) {
	fap_rand();
	fap_png(pixels, row_size, width, height, pixel_fapper_rand);
}