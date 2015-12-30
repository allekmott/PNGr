/* pngen.c
 * Created: 26 Dec 2014
 */

#include <png.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "pngen.h"

void seed_rand() {
	static int seeded_rand = 0;
	if (!seeded_rand++)
		srand(time(NULL));
}

void gen_png(png_bytep *pixels, struct image_info *info, void (*pixgen) (struct pixel *)) {
	/* to create image of width and height:
	 * height rows of width pixels
	 * row_size / width = bitdepth
	 */

	int rown;

	/* for each row */
	for (rown = 0; rown < info->height; rown++) {
		pixels[rown] = (png_bytep) malloc(info->row_size);
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

			/* generate pixel */
			pixgen(&pixel);
		}
	}
}

void pixgen_rand(struct pixel *pixel) {
	int byten;
	for (byten = 0; byten < (pixel->info->bpp); byten++)
		pixel->data[byten] = (png_byte) (rand() % 256);
}

void gen_png_rand(png_bytep *pixels, struct image_info *info) {
	seed_rand();
	gen_png(pixels, info, pixgen_rand);
}

void pixgen_sin(struct pixel *pixel) {
	/* inputs to vector function */
	float t, s;

	/* normalize or not? */
	static int normalize = 1;

	/* magnitude of zoom (in z times normal) */
	static float zoom = 1.0f;

	if (normalize) {
		/* normalize to fit exactly one cycle into image */

		/* t is horizontal parameterization */
		//static int min_t_raw = 0;
		int max_t_raw = pixel->info->width;

		/* s is vertical parameterization */
		//static int min_s_raw = 0;
		int max_s_raw = pixel->info->height;

		/* Desired range of inputs:
		 * t: 0 -> 2pi
		 * s: 0 -> 2pi
		 * Adjustment:
		 * max_norm_t = 1 -> max_t_raw / max_t_raw
		 * t(x) = pi * (x / max_t_raw)
		 * 
		 * max_norm_s = 1 -> max_s_raw / max_s_raw
		 * s(y) = pi * (y / max_s_raw)
		 */

		/* pixel's norm'd values */
		t = 2.0f * M_PI * ((float) (pixel->x) / (float) max_t_raw);
		s = 2.0f * M_PI * ((float) (pixel->y) / (float) max_s_raw);

	} else {
		/* just map to raw pixel posistions */
		t = pixel->x;
		s = pixel->y;
	}

	/* adjust to zoom */
	t *= 1.0f / zoom;
	s *= 1.0f / zoom;


	/* plug values into function */

	/* now, we need to differentiate between grayscale and color
	 * images.
	 */
	if (pixel->info->color) {
		png_byte colorVals[pixel->info->bpp];

		png_byte redValue,
			greenValue,
			blueValue;

		/* define vector function f that takes 2 inputs (t, s)
		 * and yields a 3-dimensional output vector
		 * f(t,s) = <r, g, b>
		 * (eventually may encapsulate this into generic function
		 * pointer)
		 */
		redValue = (png_byte) (127.0f * sin(t + pow(s, t))  + 128.0f);
		greenValue = (png_byte) (127.0f * sin(s + pow(t, s)) + 128.0f);
		blueValue = (png_byte) (127.0f * sin(t/s) + 128.0f);

			//redValue *= sin(greenValue);
			//greenValue *= sin(blueValue);
			//blueValue *= sin(redValue);

		colorVals[0] = redValue;
		colorVals[1] = greenValue;
		colorVals[2] = blueValue;

		/* if alpha, throw in a nifty constant */
		if (pixel->info->bpp > 3)
			colorVals[3] = (png_byte) 255;

		int byten;
		for (byten = 0; byten < (pixel->info->bpp); byten++)
			pixel->data[byten] = colorVals[byten];
	} else {
		/* TODO define f(t, s) */
		int colorVal = 256 * sin(t) - 256 * sin(s);

		int byten;
		for (byten = 0; byten < (pixel->info->bpp); byten++)
			pixel->data[byten] = (png_byte) colorVal;
	}
	
}

void gen_png_sin(png_bytep *pixels, struct image_info *info) {
	gen_png(pixels, info, pixgen_sin);
}
