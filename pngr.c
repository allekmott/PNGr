/* pngr.c
 * Created: 26 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PNG_DEBUG 3
#include <png.h>

#include "pngen.h"
#include "pngr.h"

#define PNGR_VERSION "0.2.0"

void version() {
	printf("PNGr v%s\n", PNGR_VERSION);
}

void usage(const char *exe) {
	printf("Usage: %s\n", exe);
	exit(0);
}

void gtfo(const char *msg) {
	fprintf(stderr, "%s.\n", msg);
	abort();
}

enum palette derive_palette(const char *input) {
	int numerical = atoi(input);
	if (numerical < 0 || numerical > 3) {
		printf("Invalid palette value: %i\n", numerical);
		exit(0);
	}

	return numerical;
}

const char *palette_string(enum palette palette) {
	switch (palette) {
		case RGB_24: return "24-bit RGB";
		case RGBA_32: return "32-bit RGBa";
		case GRAYSCALE_A: return "Grayscale (with alpha)";
		case GRAYSCALE: return "Grayscale (without alpha)";
		default: gtfo("Invalid palette");
	}
}

png_byte palette_pngequiv(enum palette palette) {
	switch (palette) {
		case RGB_24: return PNG_COLOR_TYPE_RGB;
		case RGBA_32: return PNG_COLOR_TYPE_RGB_ALPHA;
		case GRAYSCALE_A: return PNG_COLOR_TYPE_GRAY_ALPHA;
		case GRAYSCALE: return PNG_COLOR_TYPE_GRAY;
		default: gtfo("Invalid palette");
	}
}

png_byte palette_bitdepth(enum palette palette) {
	return (png_byte) 8;
}

int palette_color(enum palette palette) {
	switch (palette) {
		case RGB_24:
		case RGBA_32: return 1;
		case GRAYSCALE_A:
		case GRAYSCALE: return 0;
		default: gtfo("Invalid palette");
	}
}

int main(int argc, char *argv[]) {
	version();

	/* name of prospective PNG */
	char *filename = "lol.png";
	enum palette color_palette = RGB_24;
	int image_width = 500,
		image_height = 500;

	void (*png_gen) (png_bytep *, struct image_info *) = gen_png_sin;

	FILE *png_file;
	
	int c;
	/* the beautiful getopt */
	while ((c = getopt(argc, argv, "a:o:w:h:p:")) != -1) {
		switch (c) {
			case 'a': /* switch alorightms */
				if (optarg[0] == 'r') {
					printf("Using random color algorithm\n");
					png_gen = gen_png_rand;
				} else if (optarg[0] == 's') {
					printf("Using sinusoidal color algorithm\n");
					png_gen = gen_png_sin;
				} else
					usage(argv[0]);
				break;
			case 'o': filename = optarg; break;
			case 'w': image_width = atoi(optarg); break;
			case 'h': image_height = atoi(optarg); break;
			case 'p': color_palette = derive_palette(optarg); break;
			case '?': usage(argv[0]); break;
		}
	}

	printf("Input parameters:\n\n"
		"\tOutput file:\t%s\n"
		"\tDimensions:\t%i x %i\n"
		"\tColor palette:\t%s\n\n",
		filename, image_width, image_height,
		palette_string(color_palette));

	png_file = fopen(filename, "wb");
	if (png_file == NULL)
		gtfo("Could not open file for writing");

	png_structp png_ptr;
	png_infop info_ptr;

	/* attempt creation of png write struct */
	if (!(png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)))
		gtfo("png_create_png_ptr failed");

	/* attempt creation of png info struct */
	if (!(info_ptr = png_create_info_struct(png_ptr)))
		gtfo("png_create_info_struct failed");

	/* attempt to initialize I/O */
	if (setjmp(png_jmpbuf(png_ptr)))
		gtfo("Unable to initalize I/O");

	png_init_io(png_ptr, png_file);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		gtfo("Unable to write header");

	png_set_IHDR(png_ptr, info_ptr, image_width, image_height,
		palette_bitdepth(color_palette), palette_pngequiv(color_palette),
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write image */
	if (setjmp(png_jmpbuf(png_ptr)))
		gtfo("Unable to write image");

	png_bytep *pixels = malloc(sizeof(png_bytep) * image_height);

	struct image_info info;
	info.width = image_width;
	info.height = image_height;
	info.row_size = png_get_rowbytes(png_ptr, info_ptr);
	info.bpp = info.row_size / info.width;
	info.color = palette_color(color_palette);

	printf("Generating image...\n");
	png_gen(pixels, &info);

	printf("Generation complete, writing...\n");
	png_write_image(png_ptr, pixels);
	printf("Write complete. Exiting\n");

	fclose(png_file);
	free(pixels);
	return 0;
}