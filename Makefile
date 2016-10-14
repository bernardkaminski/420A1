TARGETS = rectify pool convolve
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -std=c99 -fopenmp

.PHONY: default all clean

default: $(TARGETS)
all: default

pool:
	gcc pooling.c lodepng.c -o pool -std=c99 -lm -fopenmp
	
rectify:
	gcc q1.c lodepng.c -o rectify -std=c99 -lm -fopenmp
	
convolve:
	gcc convolution.c lodepng.c -o convolve -std=c99 -lm -fopenmp

clean:
	-rm -f *.o
	-rm -f $(TARGETS)