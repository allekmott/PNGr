/* pngr.c
 * Created: 26 Dec 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include "pngr.h"

#define PNGR_VERSION "0.0.3"

void version() {
	printf("PNGr v%s\n", PNGR_VERSION);
}

void usage(const char *exe) {
	printf("Usage: %s\n", exe);
	exit(0);
}

void gtfo(const char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
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
		gtfo("Could not open file for writing.");



	fclose(png_file);
	return 0;
}