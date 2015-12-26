/* pngr.h
 * Created: 26 Dec 2015
 */ 


#ifndef PNGR_H
#define PNGR_H

/* enum for palette type */
enum palette {
	RGB_24,
	RGBA_32,
	GRAYSCALE_A,
	GRAYSCALE
};

/* print version information */
void version();

/* display usage information (user = pl3b) */
void usage(const char *exe);

/* gtfo - display error message and exit */
void gtfo(const char *msg);

/* derive palette value from textual input */
enum palette derive_palette(const char *input);

/* return string value of palette enumeration */
const char *palette_string(enum palette palette);

#endif