/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("file open failure\n");
        return NULL;
    }

    char buf[60];
    uint32_t cols, rows;
    fscanf(fp, "%s", buf); // read file format. not used
    fscanf(fp, "%d %d", cols, rows); // read col and row
    fscanf(fp, "%s", buf); // read scale. not used.

    if (!(cols && rows)) {
        printf("col/row cannot be zero\n");
        return NULL;
    }

    uint32_t numPixels = cols * rows;
    if (numPixels / cols != rows) {
        printf("uint32_t multiplication overflow\n");
        return NULL;
    }

    Image *img = malloc(sizeof(Image) + sizeof(Color) * numPixels);
    img->cols = cols;
    img->rows = rows;

    uint32_t idx = 0;
    uint8_t r, g, b;
    while (fscanf(fp, "%d %d %d", r, g, b)) {
        Color *color = malloc(sizeof(Color));
        color->R = r;
        color->G = g;
        color->B = b;
        (img->image)[idx++] = color;
    }

    fclose(fp);

    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{

}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
}