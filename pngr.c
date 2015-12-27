/* pngr.c
 * Created: 26 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <png.h>

#include "pngr.h"

#define PNGR_VERSION "0.0.4"

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
		default: return "Unknown palette type";
	}
}

png_byte palette_pngequiv(enum palette palette) {
	switch (palette) {
		case RGB_24: return PNG_COLOR_TYPE_RGB;
		case RGBA_32: return PNG_COLOR_TYPE_RGB_ALPHA;
		case GRAYSCALE_A: return PNG_COLOR_TYPE_GRAY_ALPHA;
		case GRAYSCALE: return PNG_COLOR_TYPE_GRAY;
		default: gtfo("wtf m8");
	}
}

png_byte palette_bitdepth(enum palette palette) {
	return (png_byte) 8;
}

int main(int argc, char *argv[]) {
	version();

	/* name of prospective PNG */
	char *filename = "lol.png";
	enum palette color_palette = RGB_24;
	int image_width = 500,
		image_height = 500;

	FILE *png_file;
	
	int c;
	/* the beautiful getopt */
	while ((c = getopt(argc, argv, "o:w:h:p:")) != -1) {
		switch (c) {
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
	if (!(png_ptr = png_create_png_ptr(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)))
		gtfo("png_create_png_ptr failed");

	/* attempt creation of png info struct */
	if (!(info_ptr = png_create_info_struct(png_ptr)))
		gtfo("png_create_info_struct failed");

	/* attempt to initialize I/O */
	if (setjmp(png_jmpbuf(png_ptr)))
		gtfo("Unable to initalize I/O");

	png_init_io(png_ptr, info_ptr);


	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
		gtfo("Unable to write header");

	png_set_IHDR(png_ptr, info_ptr, image_width, image_height,
		palette_bitdepth(palette), palette_pngequiv(palette),
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write image */
	if (setjmp(png_jmpbuf(png_ptr)))
		gtfo("Unable to write image");

	png_bytep pixels[image_height];

	fap_png(pixels, png_get_rowbytes(png_ptr, info_ptr), width, height);
	png_write_image(png_ptr, pixels);

	fclose(png_file);
	return 0;
}