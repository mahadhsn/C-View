#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blur.h"

////////////////////////////////////////
// Function to blur an image
// arguments: FILE *in, FILE *out the input and output files
// returns: none
////////////////////////////////////////

// Blurs an image by averaging out the colors in a 3x3 grid around each pixel
void blur(FILE *in, FILE *out) {
    unsigned char header[54];

    // Read the BMP header and write it to the output file
    fread(header, sizeof(unsigned char), 54, in);
    fwrite(header, sizeof(unsigned char), 54, out);

    // Get the width, height, and padding from the BMP header
    unsigned int width = *(unsigned int *)&header[18];
    unsigned int height = *(unsigned int *)&header[22];
    unsigned int padding = (4 - (width * 3) % 4) % 4;

    // Allocate memory for the original image data
    unsigned char *imageData = (unsigned char *)malloc((width * 3 + padding) * height);
    if (!imageData) {
        fprintf(stderr, "Oops! Couldn't allocate memory for the image data.\n");
        return;
    }

    fread(imageData, sizeof(unsigned char), (width * 3 + padding) * height, in);

    // Allocate memory for the blurred image data
    unsigned char *blurredData = (unsigned char *)malloc((width * 3 + padding) * height);
    if (!blurredData) {
        free(imageData);
        fprintf(stderr, "Not enough memory for the blurred image. Try again later.\n");
        return;
    }

    // This part does the actual blurring
    int kernelSize = 5; // The size of the blur grid (5x5)
    int kernelHalf = kernelSize / 2;

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            int rSum = 0, gSum = 0, bSum = 0;
            int pixelCount = 0;

            // Go through the 5x5 grid around the pixel
            for (int ky = -kernelHalf; ky <= kernelHalf; ky++) {
                for (int kx = -kernelHalf; kx <= kernelHalf; kx++) {
                    int pixelX = x + kx;
                    int pixelY = y + ky;

                    // Make sure we don't go out of bounds
                    if (pixelX >= 0 && pixelX < width && pixelY >= 0 && pixelY < height) {
                        unsigned char *currentPixel =
                            &imageData[(pixelY * (width * 3 + padding)) + (pixelX * 3)];

                        bSum += currentPixel[0];
                        gSum += currentPixel[1];
                        rSum += currentPixel[2];
                        pixelCount++;
                    }
                }
            }

            // Average out the colors in the grid
            unsigned char *blurredPixel = &blurredData[(y * (width * 3 + padding)) + (x * 3)];
            blurredPixel[0] = bSum / pixelCount;
            blurredPixel[1] = gSum / pixelCount;
            blurredPixel[2] = rSum / pixelCount;
        }
    }

    // Write the blurred image to the output file
    fwrite(blurredData, sizeof(unsigned char), (width * 3 + padding) * height, out);

    // Free the memory we used
    free(imageData);
    free(blurredData);
}

