#include <stdio.h>
#include <stdlib.h>

void edgeDetect(FILE *in, FILE *out) {
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

    // Allocate memory for edge data, including padding
    unsigned char* edgeData = (unsigned char*)malloc((width * 3 + padding) * height);
    if (edgeData == NULL) {
        //printf("Error allocating memory for edge data\n");
        free(imageData);
        return;
    }

    // Edge detection
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                unsigned char pixel = imageData[i * (width * 3 + padding) + j * 3 + k];
                unsigned char leftPixel = j > 0 ? imageData[i * (width * 3 + padding) + (j - 1) * 3 + k] : 0;
                unsigned char rightPixel = j < width - 1 ? imageData[i * (width * 3 + padding) + (j + 1) * 3 + k] : 0;
                unsigned char topPixel = i > 0 ? imageData[(i - 1) * (width * 3 + padding) + j * 3 + k] : 0;
                unsigned char bottomPixel = i < height - 1 ? imageData[(i + 1) * (width * 3 + padding) + j * 3 + k] : 0;

                int edge = 5 * pixel - leftPixel - rightPixel - topPixel - bottomPixel;
                edge = edge < 0 ? 0 : edge;
                edge = edge > 255 ? 255 : edge;

                edgeData[i * (width * 3 + padding) + j * 3 + k] = edge;
            }
        }
    }

    // Write edge data to output
    for (unsigned int i = 0; i < height; i++) {
        if (fwrite(edgeData + i * (width * 3 + padding), sizeof(unsigned char), width * 3, out) != width * 3) {
            //printf("Error writing edge data\n");
            free(imageData);
            free(edgeData);
            return;
        }
        for (unsigned int j = 0; j < padding; j++) {
            fputc(0, out);  // Write padding bytes
        }
    }

    free(imageData);
    free(edgeData);
}