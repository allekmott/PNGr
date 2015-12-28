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

void fap_png(png_bytep *pixels, struct image_info *info, void (*pixel_fapper) (struct pixel *)) {
	/* to create image of width and height:
	 * height rows of width pixels
	 * row_size / width = bitdepth
	 */

	int rown;

	/* for each row */
	for (rown = 0; rown < info->height; rown++) {
		pixels[rown] = malloc(info->row_size);
		png_bytep row = pixels[rown];

		int pixeln;

		/* for each pixel in the row */
		for (pixeln = 0; pixeln < info->width; pixeln++) {
			/* create pixel struct */
			struct pixel pixel;
			pixel.x = pixeln;
			pixel.y = rown;
			pixel.info = info;
			pixel.data = &(row[pixeln * info->bpp]);

			/* fap dat pixel! */
			pixel_fapper(&pixel);
		}
	}
}

void pixel_fapper_rand(struct pixel *pixel) {
	int byten;
	for (byten = 0; byten < (pixel->info->bpp); byten++)
		pixel->data[byten] = (png_byte) (rand() % 256);
}

void fap_png_rand(png_bytep *pixels, struct image_info *info) {
	fap_rand();
	fap_png(pixels, info, pixel_fapper_rand);
}

void pixel_fapper_sin(struct pixel *pixel) {
	/* normalize to fit exactly one cycle into image */

	/* t is horizontal parameterization */
	int min_t_raw = 0;
	int max_t_raw = pixel->info->width;

	/* s is vertical parameterization */
	int min_s_raw = 0;
	int max_s_raw = pixel->info->height;

	/* Desired range of inputs:
	 * t: 0 -> pi
	 * s: 0 -> pi
	 * Adjustment:
	 * max_norm_t = 1 -> max_t_raw / max_t_raw
	 * t(x) = pi * (x / max_t_raw)
	 * 
	 * max_norm_s = 1 -> max_s_raw / max_s_raw
	 * s(y) = pi * (y / max_s_raw)
	 */

	/* pixel's norm'd values */
	float t = M_PI * ((float) (pixel->x) / (float) max_t_raw);
	float s = M_PI * ((float) (pixel->y) / (float) max_s_raw);

	/* TODO define f(t, s) */
	int colorVal = (int) 256 * sin(t) - (int) 256 * sin(s);

	int byten;
	for (byten = 0; byten < (pixel->info->bpp); byten++)
		pixel->data[byten] = colorVal;
}

void fap_png_sin(png_bytep *pixels, struct image_info *info) {
	fap_png(pixels, info, pixel_fapper_sin);
}
