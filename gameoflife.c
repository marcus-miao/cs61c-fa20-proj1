/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
    Color *nextColor = malloc(sizeof(Color));
    nextColor->R = nextColor->G = nextColor->B = 0;

    const int numSurrounding = 8;
    int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

    const int colorSize = 8;
    int aliveCountR[8] = {0}, aliveCountG[8] = {0}, aliveCountB[8] = {0};
    for (int i = 0; i < numSurrounding; i++) {
        // get index of the neighbor in the matrix, with consideration of wrapping
        int neighborCol = (col + dx[i]) % image->cols;
        neighborCol = neighborCol < 0 ? neighborCol + image->cols : neighborCol;
        int neighborRow = (row + dy[i]) % image->rows;
        neighborRow = neighborRow < 0 ? neighborRow + image->rows : neighborRow;

        Color *neighborColor = image->image[neighborRow * image->cols + neighborCol];

        // bit-wise alive count
        for (int j = 0; j < colorSize; j++) {
            int mask = 1 << j;
            aliveCountR[j] += (neighborColor->R & mask) > 0; 
            aliveCountG[j] += (neighborColor->G & mask) > 0;
            aliveCountB[j] += (neighborColor->B & mask) > 0;
        }
    }

    Color *currentColor = image->image[row * image->cols + col];
    const int shift = 9;
    for (int i = 0; i < colorSize; i++) {
        int mask = 1 << i;
        int stateIdxR = ((currentColor->R & mask) > 0) * shift + aliveCountR[i];
        int stateIdxG = ((currentColor->G & mask) > 0) * shift + aliveCountG[i];
        int stateIdxB = ((currentColor->B & mask) > 0) * shift + aliveCountB[i];

        nextColor->R += ((rule & 1 << stateIdxR) > 0) << i;
        nextColor->G += ((rule & 1 << stateIdxG) > 0) << i;
        nextColor->B += ((rule & 1 << stateIdxB) > 0) << i;
    }

    return nextColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
    Image *nextImage = malloc(sizeof(Image));
    nextImage->rows = image->rows;
    nextImage->cols = image->cols;

    uint64_t numPixels = image->rows * image->cols;
    nextImage->image = malloc(sizeof(Color*) * numPixels);

    for (int row = 0; row < image->rows; row++) {
        for (int col = 0; col < image->cols; col++) {
            Color *nextColor = evaluateOneCell(image, row, col, rule);            
            nextImage->image[row * image->cols + col] = nextColor;
        }
    }

    return nextImage;
}

void processCLI(int argc, char **argv, char **filename, uint32_t *rule) 
{
	if (argc != 3) {
		printf("usage: %s filename rule\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		exit(-1);
	}
	*filename = argv[1];
    *rule = strtoul(argv[2], NULL, 0);
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
    char *fileName;
    uint32_t rule;
    processCLI(argc, argv, &fileName, &rule);
    
    Image *image = readData(fileName);
    Image *nextImage = life(image, rule);
    writeData(nextImage);

    freeImage(image);
    freeImage(nextImage);
}
