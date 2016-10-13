/* Example of using lodepng to load, process, save image */
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
 #include <math.h>


//	#define max(a,b) ((a) > (b) ? (a) : (b))

int max(int num1, int num2)
{
	int result;
	if (num1 > num2)
	{
		result = num1;
	}
	else
	{
		result = num2;
	}
	return result;
}



void pool(int pixel,int address,unsigned char * new_image) {

}


void process(char* input_filename, char* output_filename,char* threads)
{
  unsigned error;
  unsigned char *image, *new_image;
  unsigned width, height;


  error = lodepng_decode32_file(&image, &width, &height, input_filename);
  unsigned char *keepTrack = (unsigned char *)malloc(width*height*4*sizeof(unsigned char));

  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  new_image = malloc(width * height * 4 * sizeof(unsigned char));

   memcpy(keepTrack, image, strlen(new_image)+1);

  // process image
  unsigned char value;

//omp_set_num_threads(atoi(threads));
//#pragma omp parallel for collapse(2)
  for (int i = 0; i < height; i+=2) {
    for (int j = 0; j < width; j+=2) {


    	// add a bit vector or boolean vector to make sure we don't calculate that region??

    	// get all adjacent squares
    	int main = 4*width*i + 4*j;
    	int down = 4*width*(i+1) + 4*j;
		int right = 4*width*i + 4*(j+1);
		int diagonal = 4*width*(i+1) + 4*(j+1);



		int maxR = max( max(image[main], image[down]), max(image[right], image[diagonal]));
		int maxG = max( max(image[main+1], image[down+1]), max(image[right+1], image[diagonal+1]));
		int maxB = max( max(image[main+2], image[down+2]), max(image[right+2], image[diagonal+2] ));


		// how do we know how to shrink the original image?
		new_image[4*width*i + 4*j] = maxR;
		new_image[(4*width*i + (4*j)) + 1] = maxG;
		new_image[(4*width*i + (4*j)) + 2] = maxB;
//		new_image[main+3] = 255;

//	    int r = image[offset + 0]; // R
//	    rec(r,(offset + 0),new_image);
//	    int g =image[offset + 1]; // G
//	    rec(g,offset+1,new_image);
//	    int b =image[offset + 2]; // B
//	    rec(b,offset+2,new_image);
//	    int four = image[offset + 3]; // A
//	    new_image[offset + 3]=r;
    }
  }


  lodepng_encode32_file(output_filename, new_image, width/2, height/2);

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
