/*
 * q1.c
 *
 *  Created on: Oct 12, 2016
 *      Author: bernie
 */
/* Example of using lodepng to load, process, save image */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
 #include <math.h>


void rec(int pixel,int address,unsigned char * new_image) {
	if (pixel<127) {
		new_image[address]=127;
	}
	else{
		new_image[address]=pixel;
	}
}


void process(char* input_filename, char* output_filename,char* threads)
{
  unsigned error;
  unsigned char *image, *new_image;
  unsigned width, height;

  error = lodepng_decode32_file(&image, &width, &height, input_filename);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  new_image = malloc(width * height * 4 * sizeof(unsigned char));

  // process image
  unsigned char value;
  int xyz = 123; // more computations
omp_set_num_threads(atoi(threads));
#pragma omp parallel for collapse(2)
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
    	// printf("thread num: %d\n", omp_get_num_threads());

    	// printf("thread num: %d\n", omp_get_thread_num());
    	int offset = 4*width*i + 4*j;
	    int r =image[offset + 0]; // R
	    rec(r,(offset + 0),new_image);
	    int g =image[offset + 1]; // G
	    rec(g,offset+1,new_image);
	    int b =image[offset + 2]; // B
	    rec(b,offset+2,new_image);
	    int a = image[offset + 3]; // A
	    new_image[offset + 3]=a;
	    xyz = pow(sin(i+xyz), cos(xyz + j));
    }
  }

  lodepng_encode32_file(output_filename, new_image, width, height);

  free(image);
  free(new_image);
}

int main(int argc, char *argv[])
{
  char* input_filename = argv[1];
  char* output_filename = argv[2];

  process(input_filename, output_filename,argv[3]);

  return 0;
}
