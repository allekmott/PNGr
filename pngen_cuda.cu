#include <iostream>
#include <iomanip>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <png.h>

#include "pngr.h"
#include "pngen.h"
#include "pngen_cuda.h"

/* util */
#define checkCudaErrors(val) check( (val), #val, __FILE__, __LINE__)

template<typename T>
void check(T err, const char* const func, const char* const file, const int line) {
  if (err != cudaSuccess) {
    std::cerr << "CUDA error at: " << file << ":" << line << std::endl;
    std::cerr << cudaGetErrorString(err) << " " << func << std::endl;
    exit(1);
  }
}


/* kernel for pixel generation */
__global__
void cudapixgen_sin(png_bytep pixel_bytes, int width, int height, int bpp) {
	const int2 thread_2D_pos = make_int2(blockIdx.x * blockDim.x + threadIdx.x,
		blockIdx.y * blockDim.y + threadIdx.y);

	int absArrayPos = thread_2D_pos.y * (width * bpp) + thread_2D_pos.x;
	if (thread_2D_pos.x > (width * bpp - 1) || thread_2D_pos.y > (height - 1))
		return;

	int x = thread_2D_pos.x,
		y = thread_2D_pos.y;

	/* t, s = normalized x, y */
	float t = 2.0 * M_PI * ((float) x / (float) (width * bpp)),
		s = 2.0 * M_PI * ((float) y / (float) height);

	/* f(t,s) = <r(t,s), g(t,s), b(t,s)>
	 * which component is the thread working on?
	 * Take horizontal position (resets with every new row)
	 * % 3 -> {0, 1, 2}
	 * 0: red
	 * 1: green
	 * 2: blue
	 */
	float pixelComp;
	switch (x % 3) {
		case 0:
			/* red */
			pixelComp = (127.0f * sin(t + tan(s - t)) + 128.0f);
			break;
		case 1:
			/* green */
			pixelComp = (127.0f * sin(s + tan(t - s)) + 128.0f);
			break;
		case 2:
			/* blue */
			pixelComp = (127.0f * sin(t*s) + 128.0f);
			break;
	}
	pixel_bytes[absArrayPos] = (png_byte) pixelComp;
}

void cudagen_png_sin(png_bytep *pixels, struct image_info *info) {
	/* h_pixels is a pointer to row pointers
	 * we still have to allocate memory for the individual pixels
	 */
	size_t numPixelBytes = info->row_size * info->height;
	png_bytep h_pixel_bytes = (png_bytep) malloc(numPixelBytes);
	if (h_pixel_bytes == NULL)
		gtfo("Unable to allocate sufficient memory");

	/* modify row pointer to point to these rows */
	int rown;
	for (rown = 0; rown < info->height; rown++)
		pixels[rown] = &(h_pixel_bytes[rown * info->row_size]);


	/* pointer to pixel array on device */
	png_bytep d_pixel_bytes;

	/* Allocate memory on GPU */
	checkCudaErrors(cudaMalloc(&d_pixel_bytes, numPixelBytes));

	/* Block sizing */
	const dim3 block_size(48, 16, 1);

	/* Grid sizing */
	const dim3 grid_size((info->width/16) + 1, (info->height/16) + 1, 1);

	/* Launch kernel */
	cudapixgen_sin<<<grid_size, block_size>>>(d_pixel_bytes, info->width, info->height, info->bpp);

	/* wait for kernel to complete execution, then check for errors */
	cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());

	/* Copy results from device -> host */
	checkCudaErrors(cudaMemcpy(h_pixel_bytes, d_pixel_bytes, numPixelBytes, cudaMemcpyDeviceToHost));

	checkCudaErrors(cudaFree(d_pixel_bytes));
}