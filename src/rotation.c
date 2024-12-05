#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////
// Function to rotate an image 90 degrees to the left
// arguments: FILE *in, FILE *out the input and output files
// returns: none
////////////////////////////////////////

void rotateLeft90(FILE *in, FILE *out) {
    unsigned char imageHeader[54];

    // Read the BMP header
    if (fread(imageHeader, sizeof(unsigned char), 54, in) != 54) {
        //printf("Error reading image header\n");
        return;
    }

    // Extract original dimensions and padding
    unsigned int width = *(unsigned int*)&imageHeader[18];
    unsigned int height = *(unsigned int*)&imageHeader[22];
    unsigned int padding = (4 - (width * 3) % 4) % 4;

    // Update header dimensions for the rotated image
    *(unsigned int*)&imageHeader[18] = height; // New width
    *(unsigned int*)&imageHeader[22] = width; // New height
    unsigned int newPadding = (4 - (height * 3) % 4) % 4;

    // Write the updated header to the output file
    if (fwrite(imageHeader, sizeof(unsigned char), 54, out) != 54) {
        //printf("Error writing image header\n");
        return;
    }

    // Allocate memory for the input image data
    unsigned char* imageData = (unsigned char*)malloc((width * 3 + padding) * height);
    if (imageData == NULL) {
        //printf("Error allocating memory for image data\n");
        return;
    }

    // Read the pixel data from the input file
    if (fread(imageData, sizeof(unsigned char), (width * 3 + padding) * height, in) != (width * 3 + padding) * height) {
        //printf("Error reading image data\n");
        free(imageData);
        return;
    }

    // Allocate memory for the rotated image data
    unsigned char* rotatedData = (unsigned char*)malloc((height * 3 + newPadding) * width);
    if (rotatedData == NULL) {
        //printf("Error allocating memory for rotated data\n");
        free(imageData);
        return;
    }

    // Perform the rotation
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                rotatedData[(j * (height * 3 + newPadding)) + (height - i - 1) * 3 + k] =
                    imageData[i * (width * 3 + padding) + j * 3 + k];
            }
        }
    }

    // Write the rotated image data to the output file
    for (unsigned int i = 0; i < width; i++) {
        if (fwrite(rotatedData + i * (height * 3 + newPadding), sizeof(unsigned char), height * 3, out) != height * 3) {
            //printf("Error writing rotated data\n");
            free(imageData);
            free(rotatedData);
            return;
        }
        // Write the new padding
        for (unsigned int j = 0; j < newPadding; j++) {
            fputc(0, out);
        }
    }

    // Clean up
    free(imageData);
    free(rotatedData);

    //printf("Rotation completed successfully\n");
}