/*
 *  main.c
 *  TCSS 333 - Autumn 2015
 *
 *  Assignment 4 - Image Manipulation Without Arrays
 *  Alex Terikov (teraliv@uw.edu)
 *  10/29/15
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


int checkFilesAndMakeImages(const char *fname1, const char *fname2);
void imageDoubleExposure(unsigned char *img1, unsigned char *img2, long size);
void imageCheckerBoard(unsigned char *img1, unsigned char *img2, long size);
unsigned long getFileSize(char *file);


int main(int argc, const char **argv) {

    // get file names
    const char *fname1 = *++argv;
    const char *fname2 = *++argv;

    
    // check if anything wrong with a file or bitmap data
    if (!checkFilesAndMakeImages(fname1, fname2)) {
        printf("File does not exist or incorrect!\n");
        exit(0);
    }
    
    // if both files are good make image manipulations
    // and create two new BMP images
    else {
        checkFilesAndMakeImages(fname1, fname2);
    }
    
    return 0;
}


/*
 * This function check if both files were open correctly
 * and checks if the data is correct for a bitmap image file.
 *
 * @param *fname1 - file name for the first image
 * @param *fname2 - file name for the second image
 * @return - returns 0 if the file is missing ot incorrect, otherwise 1.
 */
int checkFilesAndMakeImages(const char *fname1, const char *fname2) {
    
    // open BMP files in a binary mode
    FILE *bmpFile1 = fopen(fname1, "rb");
    FILE *bmpFile2 = fopen(fname2, "rb");
    
    int result = 0;
    
    unsigned char *image1;
    unsigned char *image2;
    
    unsigned long fsize1; // 1st file size
    unsigned long fsize2; // 2nd file size
    
    // check if both files were open
    if (bmpFile1 && bmpFile2) {
        
        // get the sizes of both files
        fsize1 = getFileSize((char *) fname1);
        fsize2 = getFileSize((char *) fname2);
        
        // compare if the file sizes are the same
        if (fsize1 == fsize2) {
            image1 = malloc(fsize1);
            image2 = malloc(fsize2);
            
            unsigned char *p1 = image1;
            unsigned char *p2 = image2;
            
            fread(image1, sizeof(char), fsize1, bmpFile1);
            fread(image2, sizeof(char), fsize2, bmpFile2);
            
            // Here we check BMP data by comparing first 2 byts:
            // 1st byte: B
            // 2nd byte: M
            if (*p1 == 'B' && *p2 == 'B' && *++p1 == 'M' && *++p2 == 'M') {
                imageDoubleExposure(image1, image2, fsize1);
                imageCheckerBoard(image1, image2, fsize2);
                
                result = 1;
            }
            
            free(image1);
            free(image2);
        }
    }
    
    fclose(bmpFile1);
    fclose(bmpFile2);
    
    return result;
}


/*
 * Creates a bitmap image file with double exposure
 * effect based on two images.
 *
 * @param *image1 - pointer to the 1st image array
 * @param *image2 - pointer to the 2nd image array
 */
void imageDoubleExposure(unsigned char *img1, unsigned char *img2, long size) {
    
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
    fwrite(image, sizeof(char), size, doubleExposureFile);
    
    free(image);
    fclose(doubleExposureFile);
}


/*
 * Creates a bitmap image file with checker board
 * effect based on two images.
 *
 * @param *image1 - pointer to the 1st image array
 * @param *image2 - pointer to the 2nd image array
 */
void imageCheckerBoard(unsigned char *img1, unsigned char *img2, long size) {
    
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
    
    // write bmp checker board data to the file
    fwrite(image, sizeof(char), size, checkerBoardFile);
    
    free(image);
    fclose(checkerBoardFile);
}

/*
 * Gets the size of the file.
 *
 * @param *file - the file to calculate the size.
 * @return - returns the file size, or -1 if the file is bad.
 */
unsigned long getFileSize(char *file) {
    long result = -1;
    struct stat file_info;
    if (stat(file, &file_info) > -1)
    {
        result = (unsigned long)(file_info.st_size);
    }
    return (unsigned long)result;
}