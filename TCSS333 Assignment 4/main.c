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
#include <sys/stat.h>

void imageDoubleExposure(unsigned char *img1, unsigned char *img2, int size);
void imageCheckerBoard(unsigned char *image1, unsigned char *image2, int size);
void readParameters(int num, char **param);
unsigned long getFileSize(char *file);
int compareFileExtensions(char *ext1, char *ext2);


int main(int argc, const char **argv) {
    
//    readParameters(argc, (char **)argv);
    
    char **p = (char **) argv;
    char *ext1 = *++p;
    char *ext2 = *++p;

    compareFileExtensions(ext1, ext2);
    
    FILE *bmpFile1 = fopen(*(argv+1), "rb");
    FILE *bmpFile2 = fopen(*(argv+2), "rb");
    
    unsigned long fsize1 = 0; // 1st file size
    unsigned long fsize2 = 0; // 2nd file size
    
    
    // if both files exists and were open correctly
    // then get their sizes
    if (bmpFile1 && bmpFile2) {
        fsize1 = getFileSize((char *) *++argv);
        fsize2 = getFileSize((char *) *++argv);
    }
    
//    printf("%s\n", *argv);
    
    // print out error message if file not opened
    if (!bmpFile1 || !bmpFile2 || (fsize1 != fsize2) || !compareFileExtensions(ext1, ext2)) {
        printf("File does not exist or files are with different sizes!");
        exit(0);
    }
    
    
    unsigned char *image1;
    unsigned char *image2;
    
    image1 = malloc(fsize1);
    image2 = malloc(fsize2);
    
    fread(image1, sizeof(char), 499446, bmpFile1);
    fread(image2, sizeof(char), 499446, bmpFile2);
    
    imageDoubleExposure(image1, image2, (int) fsize1);
    imageCheckerBoard(image1, image2, (int) fsize1);
    
    free(image1);
    free(image2);
    
    fclose(bmpFile1);
    fclose(bmpFile2);
    
    return 0;
}


void imageDoubleExposure(unsigned char *img1, unsigned char *img2, int size) {
    
    // file for a double exposure image
    FILE *doubleExposureFile = fopen("blend.bmp", "wb");
    
    unsigned char *image;
    int i;
    
    image = malloc(size);

    // copy bmp header data
    for (i = 0; i < 54; i++) {
        *(image + i) = *(img1 + i);
    }
    
    // copy bmp double exposure data
    for (i = 54; i < size; i++) {
        *(image + i) = (*(img1 + i)/2) + (*(img2 + i)/2);
    }
    
    // write bmp double exposure data to the file
    fwrite(image, sizeof(unsigned char), size, doubleExposureFile);

    
    free(image);
    fclose(doubleExposureFile);
}

void imageCheckerBoard(unsigned char *img1, unsigned char *img2, int size) {
    
    int width = *(img1 + 18) + (*(img1 + 19) * 256);
    int block = (width * 3) / 8;
    
    FILE *checkerBoardFile = fopen("checker.bmp", "wb");
    
    unsigned char *image;
    
    int i, row, col;
    int count = 54;
    
    image = malloc(size);
    
    // copy bmp header data
    for (i = 0; i < 54; i++) {
        *(image + i) = *(img1 + i);
    }
    
    for (row = 0;  row < 8;  row++) {
        for (col = 0;  col < width;  col++) {
            i = 0;
            if ((row % 2) == (col % 2)) {
                while (i < (block)) {
                    *(image + count) = *(img1 + count);
                    count++;
                    i++;
                }
            
            } else {
                while (i < (block)) {
                    *(image + count) = *(img2 + count);
                    count++;
                    i++;
                }
            }
        }
    }

    fwrite(image, sizeof(unsigned char), size, checkerBoardFile);
    
    free(image);
    fclose(checkerBoardFile);
}

int compareFileExtensions(char *ext1, char *ext2) {

    int len1 = (int) strlen(ext1) - 1;
    int len2 = (int) strlen(ext2) - 1;
    int count = 0;
    int result = 0;
    int contin = 1;
    
    ext1 += len1;
    ext2 += len2;
    
    while (contin && count < 4) {
        if (*ext1 == *ext2)
            result = 1;
        
        if (*ext1 != *ext2) {
            result = 0;
            contin = 0;
        }
        
        count++;
        ext1--;
        ext2--;
    }
    
    return result;
}

void readParameters(int total, char **param) {
    int i;
    
    for (i = 1; i < 3; i++) {
        printf("arg %i - %s\n", i, *(param+i));
    }
//    int i = total;
//    do {
//        printf("Argumment %d: %s\n", total - i, *param++);
//    } while (--i);
}

unsigned long getFileSize(char *file) {
    long result = -1;
    struct stat file_info;
    if (stat(file, &file_info) > -1)
    {
        result = (unsigned long)(file_info.st_size);
    }
    return (unsigned long)result;
}