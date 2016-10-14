/* Example of using lodepng to load, process, save image */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <math.h>
#include "wm.h"

#define constrain(num, low, high) ((num)<(low)?(low):((num)>(high)?(high):(num)))

void postProcess(int pixel, int address, unsigned char * new_image)
{
	if (pixel < 0)
	{
		new_image[address] = 0;
	}
	else if (pixel > 255)
	{
		new_image[address] = 255;
	}
	else
	{
		new_image[address] = pixel;
	}
}




void convolution(char* input_filename, char* output_filename, char* threads)
{
	unsigned error;
	unsigned char *image, *new_image;
	unsigned width, height;

	error = lodepng_decode32_file(&image, &width, &height, input_filename);

	if (error)
		printf("error %u: %s\n", error, lodepng_error_text(error));
	new_image = malloc(width * height * 4 * sizeof(unsigned char));

//omp_set_num_threads(atoi(threads));
//#pragma omp parallel for collapse(2)
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			int sumA = 0;

			int offset = 4 * width * i + 4 * j;
			// multiply weight matrix with submatrix of image
			for (int ii = 0; ii <= 2; ii++)
			{
				for (int jj = 0; jj <= 2; jj++)
				{
					int newOffset = offset + 4*jj + 4*ii;
					int inputR = image[newOffset];
					int inputG = image[newOffset+1];
					int inputB = image[newOffset+2];

					int weight = w[ii][jj];
					sumR += (inputR * weight);
					sumG += (inputG * weight);
					sumB += (inputB * weight);
				}
			}

			// can't wrap my head around this setting stuff
			new_image[4*width*i + 4*j] = constrain(sumR, 0, 255);
			new_image[4*width*i + 4*j + 1] = constrain(sumG, 0, 255);
			new_image[4*width*i + 4*j + 2] = constrain(sumB, 0, 255);
			new_image[4*width*i + 4*j + 3] = 255;
		}
	}

	lodepng_encode32_file(output_filename, new_image, width - 2, height - 2);

	free(image);
	free(new_image);
}

int main(int argc, char *argv[])
{
	char* input_filename = argv[1];
	char* output_filename = argv[2];

	convolution(input_filename, output_filename, argv[3]);

	return 0;
}
