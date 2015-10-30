/*
 *  main.c
 *  TCSS 333 - Autumn 2015
 *
 *  Assignment 4 - Image Manipulation without Arrays
 *  Alex Terikov (teraliv@uw.edu)
 *  10/29/15
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imageDoubleExposure(unsigned char *img1, unsigned char *img2);
void imageCheckerBoard(unsigned char *image1, unsigned char *image2);


int main(int argc, const char * argv[]) {
    
    FILE *bmpFile1 = fopen("in1.bmp", "rb");
    FILE *bmpFile2 = fopen("in2.bmp", "rb");

    
    // print out error message if file not opened
    if (!bmpFile1 || !bmpFile2) {
        perror("Failed to open file");
    }
    
    unsigned char *image1;
    unsigned char *image2;
    
    image1 = malloc(499446);
    image2 = malloc(499446);
    
    fread(image1, sizeof(char), 499446, bmpFile1);
    fread(image2, sizeof(char), 499446, bmpFile2);
    
    imageDoubleExposure(image1, image2);
    imageCheckerBoard(image1, image2);
    
    free(image1);
    free(image2);
    
    fclose(bmpFile1);
    fclose(bmpFile2);
    
    return 0;
}


void imageDoubleExposure(unsigned char *img1, unsigned char *img2) {
    
    // file for a double exposure image
    FILE *doubleExposureFile = fopen("blend.bmp", "wb");
    
    unsigned char *image;
    int i;
    
    image = malloc(499446);

    // copy bmp header data
    for (i = 0; i < 54; i++) {
        *(image + i) = *(img1 + i);
    }
    
    // copy bmp double exposure data
    for (i = 54; i < 499446; i++) {
        *(image + i) = (*(img1 + i)/2) + (*(img2 + i)/2);
    }
    
    // write bmp double exposure data to the file
    fwrite(image, sizeof(unsigned char), 499446, doubleExposureFile);

    
    free(image);
    fclose(doubleExposureFile);
}

void imageCheckerBoard(unsigned char *img1, unsigned char *img2) {
    
    FILE *checkerBoardFile = fopen("checker.bmp", "wb");
    
    unsigned char *image;
    
    int i, row, col;
    int count = 54;
    
    image = malloc(499446);
    
    // copy bmp header data
    for (i = 0; i < 54; i++) {
        *(image + i) = *(img1 + i);
    }
    
    for (row = 0;  row < 8;  row++) {
        for (col = 0;  col < 408;  col++) {
            
            if ((row % 2) == (col % 2)) {
                i = 0;
                while (i < (153)) {
                    *(image + count) = *(img1 + count);
                    count++;
                    i++;
                }
            
            } else {
                i = 0;
                while (i < (153)) {
                    *(image + count) = *(img2 + count);
                    count++;
                    i++;
                }
            }
        }
    }

    fwrite(image, sizeof(unsigned char), 499446, checkerBoardFile);
    
    free(image);
    fclose(checkerBoardFile);
}
