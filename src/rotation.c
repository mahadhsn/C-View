#include <stdio.h>
#include <stdlib.h>

void rotateLeft90(FILE *in, FILE *out) {
    unsigned char imageHeader[54];
    if (fread(imageHeader, sizeof(unsigned char), 54, in) != 54) {
        //printf("Error reading image header\n");
        return;
    }
    if (fwrite(imageHeader, sizeof(unsigned char), 54, out) != 54) {
        //printf("Error writing image header\n");
        return;
    }

    unsigned int width = *(unsigned int*)&imageHeader[18];
    unsigned int height = *(unsigned int*)&imageHeader[22];
    unsigned int padding = (4 - (width * 3) % 4) % 4;

    //printf("Width: %u, Height: %u, Padding: %u\n", width, height, padding);

    unsigned char* imageData = (unsigned char*)malloc((width * 3 + padding) * height);
    if (imageData == NULL) {
        //printf("Error allocating memory for image data\n");
        return;
    }
    if (fread(imageData, sizeof(unsigned char), (width * 3 + padding) * height, in) != (width * 3 + padding) * height) {
        //printf("Error reading image data\n");
        free(imageData);
        return;
    }

    unsigned int newPadding = (4 - (height * 3) % 4) % 4;
    unsigned char* rotatedData = (unsigned char*)malloc((height * 3 + newPadding) * width);
    if (rotatedData == NULL) {
        //printf("Error allocating memory for rotated data\n");
        free(imageData);
        return;
    }

    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                rotatedData[(j * height + (height - i - 1)) * 3 + k] = imageData[(i * (width * 3 + padding) + j * 3 + k)];
            }
        }
    }

    for (unsigned int i = 0; i < width; i++) {
        if (fwrite(rotatedData + i * height * 3, sizeof(unsigned char), height * 3, out) != height * 3) {
            //printf("Error writing rotated data\n");
            free(imageData);
            free(rotatedData);
            return;
        }
        for (unsigned int j = 0; j < newPadding; j++) {
            fputc(0, out);
        }
    }

    free(imageData);
    free(rotatedData);
    //printf("Rotation completed successfully\n");
}