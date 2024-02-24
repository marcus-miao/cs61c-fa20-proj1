/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
    Color *color = malloc(sizeof(Color));
    uint8_t blue = image->image[row * image->cols + col]->B;

    const uint8_t LSB_MASK = 0x01;
    uint8_t white = blue & LSB_MASK;
    if (white) {
        color->R = 255;
        color->G = 255;
        color->B = 255;
    } else {
        color->R = 0;
        color->G = 0;
        color->B = 0;
    }

    return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
    Image *imageDecoded = malloc(sizeof(Image));
    imageDecoded->rows = image->rows;
    imageDecoded->cols = image->cols;

    uint64_t numPixels = image->rows * image->cols;
    imageDecoded->image = malloc(sizeof(Color*) * numPixels);

    for (uint32_t rowIdx = 0; rowIdx < image->rows; rowIdx++) {
        for (uint32_t colIdx = 0; colIdx < image->cols; colIdx++) {
            Color *color = evaluateOnePixel(image, rowIdx, colIdx);
            imageDecoded->image[rowIdx * image->cols + colIdx] = color;
        }
    }

    return imageDecoded;
}

void processCLI(int argc, char **argv, char **filename) 
{
	if (argc != 2) {
		printf("usage: %s filename\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		exit(-1);
	}
	*filename = argv[1];
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
    char *fileName;
    processCLI(argc, argv, &fileName);

    Image *image = readData(fileName);
    Image *imageDecoded = steganography(image);
    writeData(imageDecoded);

    freeImage(image);
    freeImage(imageDecoded);
}
