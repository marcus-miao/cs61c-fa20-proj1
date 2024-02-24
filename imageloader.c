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
    fscanf(fp, "%" PRIu32 " %" PRIu32 "", &cols, &rows); // read col and row
    fscanf(fp, "%s", buf); // read scale. not used.

    if (!(cols && rows)) {
        printf("col/row cannot be zero\n");
        return NULL;
    }

    uint64_t numPixels = cols * rows;
    if (numPixels / cols != rows) {
        printf("uint32_t multiplication overflow\n");
        return NULL;
    }

    Image *img = malloc(sizeof(Image));
    img->cols = cols;
    img->rows = rows;

    img->image = malloc(sizeof(Color*) * numPixels);
    uint32_t idx = 0;
    uint8_t r, g, b;
    while (idx < numPixels) {
        fscanf(fp, "%hhu %hhu %hhu", &r, &g, &b);
        img->image[idx] = malloc(sizeof(Color));
        img->image[idx]->R = r;
        img->image[idx]->G = g;
        img->image[idx]->B = b;
        idx++;
    }

    fclose(fp);

    return img;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
    printf("P3\n");
    printf("%d %d\n", image->cols, image->rows);
    printf("255\n");

    uint64_t numPixels = image->cols * image->rows;
    uint64_t idx = 0;
    const uint8_t SEPARATOR = 3;
    while (idx < numPixels) {
        Color *color = image->image[idx];
        printf("%*d %*d %*d", SEPARATOR, color->R, SEPARATOR, color->G, SEPARATOR, color->B);

        idx++;
        if (idx % image->cols) {
            printf("%*s", SEPARATOR, " ");
        } else {
            printf("\n");
        }
    }
}

//Frees an image
void freeImage(Image *image) 
{
    uint64_t numPixels = image->cols * image->rows;
    uint64_t idx = 0;
    while (idx < numPixels) {
        free(image->image[idx++]);
    }
    free(image->image);
    free(image);
}