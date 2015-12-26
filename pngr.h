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
void usage(char *exe);

/* gtfo - display error message and exit */
void gtfo(char *msg);

#endif