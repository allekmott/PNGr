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

void usage(char *exe) {
	printf("Usage: %s\n", exe);
	exit(0);
}

void gtfo(char *msg) {
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int main(int argc, char *argv[]) {
	version();

	/* name of prospective PNG */
	char *filename = "lol.png";

	FILE *png_file;
	
	int c;
	/* the beautiful getopt */
	while ((c = getopt(argc, argv, "o:")) != -1) {
		switch (c) {
			case 'o': filename = optarg; break;
			case '?': usage(argv[0]); break;
		}
	}

	printf("Outputting to: %s\n", filename);

	png_file = fopen(filename, "wb");
	if (png_file == NULL)
		gtfo("Could not open file for writing.");



	fclose(png_file);
	return 0;
}