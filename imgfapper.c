/* imgfapper.c
 * Created: 26 Dec 2014
 */

#include <png.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "imgfapper.h"

void fap_png(png_bytep *pixels, size_t row_size, int width, int height) {
	/* to create image of width and height:
	 * height rows of width pixels
	 * row_size / width = bitdepth
	 */

	static int seeded_rand = 0;
	if (!seeded_rand++)
		srand(time(NULL));

	int bytes_per_pixel = row_size / width;

	int rown;

	/* for each row */
	for (rown = 0; rown < height; rown++) {
		pixels[rown] = malloc(row_size);
		png_bytep row = pixels[rown];

		int pixeln;

		/* for each pixel in the row */
		for (pixeln = 0; pixeln < width; pixeln++) {
			int byten;

			/* fill each data byte with a random byte */
			for (byten = 0; byten < bytes_per_pixel; byten++) {
				/* start on left = r -> g -> b -> a (assuming big endian) */
				row[(pixeln * bytes_per_pixel) + byten] = (png_byte) (rand() % 256);
			}
		}
	}
}