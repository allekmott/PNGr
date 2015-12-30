/* pngen_cuda.h
 * Parallelize pixel generation to take advantage
 * of potential increased throughput
 */

#include "pngen.h"

#ifndef PNGEN_CUDA_H
#define PNGEN_CUDA_H

/* generates sin-produces pixel values in parallel */
void cudagen_png_sin(png_bytep *pixels, struct image_info *info);

#endif