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
	new_image = malloc((width-2) * (height-2) * 4 * sizeof(unsigned char));

//omp_set_num_threads(atoi(threads));
//#pragma omp parallel for collapse(2)
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			int offset = 4 * width * (i - 1) + 4 * (j - 1);
			int outputOffset = 4 * (width - 2) * (i - 1) + 4 * (j - 1);
			// multiply weight matrix with submatrix of image
			for (int idx = 0; idx <= 2; idx++)
			{
				int sum = 0;

				for (int x = 0; x <= 2; x++)
				{
					for (int y = 0; y <= 2; y++)
					{
						int imageOffset = offset + 4 * width * x + 4 * y + idx;
						int inputPixel = image[imageOffset];

						int weight = w[x][y];
						sum += (inputPixel * weight);
					}
				}
				// can't wrap my head around this setting stuff
				new_image[outputOffset + idx] = constrain(sum, 0, 255);
			}
			new_image[outputOffset + 3] = image[offset+3];
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
