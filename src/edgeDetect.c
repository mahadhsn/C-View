#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SOBEL_X {-1, 0, 1, -2, 0, 2, -1, 0, 1}
#define SOBEL_Y {-1, -2, -1, 0, 0, 0, 1, 2, 1}

void edgeDetect(FILE *in, FILE *out) {
    unsigned char imageHeader[54];

    // Step 1: Read the BMP header to get image dimensions and verify file type
    if (fread(imageHeader, sizeof(unsigned char), 54, in) != 54) {
        printf("Error reading image header\n");
        return;
    }

    // Check for BMP format (first two bytes should be 'B' and 'M')
    if (imageHeader[0] != 'B' || imageHeader[1] != 'M') {
        printf("Error: This is not a valid BMP file\n");
        return;
    }

    unsigned int width = *(unsigned int*)&imageHeader[18];
    unsigned int height = *(unsigned int*)&imageHeader[22];
    unsigned int padding = (4 - (width * 3) % 4) % 4;

    printf("Width: %u, Height: %u, Padding: %u\n", width, height, padding);

    // Step 2: Allocate memory for the image data (including padding)
    unsigned char* imageData = (unsigned char*)malloc((width * 3 + padding) * height);
    if (imageData == NULL) {
        printf("Error allocating memory for image data\n");
        return;
    }

    // Step 3: Read image data, including padding
    if (fread(imageData, sizeof(unsigned char), (width * 3 + padding) * height, in) != (width * 3 + padding) * height) {
        printf("Error reading image data\n");
        free(imageData);
        return;
    }

    // Step 4: Allocate memory for edge data (black and white edge image)
    unsigned char* edgeData = (unsigned char*)malloc((width * 3 + padding) * height);
    if (edgeData == NULL) {
        printf("Error allocating memory for edge data\n");
        free(imageData);
        return;
    }

    // Sobel kernels for edge detection
    int sobelX[9] = SOBEL_X;
    int sobelY[9] = SOBEL_Y;

    long totalEdgeStrength = 0;
    long edgeCount = 0;

    // Step 5: Compute edge strengths and store the magnitude
    for (unsigned int i = 1; i < height - 1; i++) {
        for (unsigned int j = 1; j < width - 1; j++) {
            int maxEdge = 0;

            // Loop through the three color channels (R, G, B)
            for (int k = 0; k < 3; k++) {
                int gx = 0, gy = 0;

                // Apply Sobel kernels for edge detection
                for (int m = -1; m <= 1; m++) {
                    for (int n = -1; n <= 1; n++) {
                        int pixel = imageData[(i + m) * (width * 3 + padding) + (j + n) * 3 + k];
                        gx += sobelX[(m + 1) * 3 + (n + 1)] * pixel;
                        gy += sobelY[(m + 1) * 3 + (n + 1)] * pixel;
                    }
                }

                // Calculate gradient magnitude (edge strength)
                int edge = sqrt(gx * gx + gy * gy);
                if (edge > maxEdge) {
                    maxEdge = edge;
                }
            }

            // Accumulate edge strength
            totalEdgeStrength += maxEdge;
            edgeCount++;
        }
    }

    // Step 6: Calculate the average edge strength
    int averageEdgeStrength = totalEdgeStrength / edgeCount;

    // Step 7: Calculate threshold based on edge strength
    // The threshold will adapt based on the image's characteristics
    int threshold = averageEdgeStrength * 2; // We multiply by 2 to make sure we only highlight strong edges

    // Ensure the threshold is not too low or high
    if (threshold < 50) {
        threshold = 50;  // Set a minimum threshold to avoid too much noise
    }

    // Step 8: Detect edges and convert to black-and-white based on the threshold
    for (unsigned int i = 1; i < height - 1; i++) {
        for (unsigned int j = 1; j < width - 1; j++) {
            int maxEdge = 0;

            // Loop through the three color channels (R, G, B)
            for (int k = 0; k < 3; k++) {
                int gx = 0, gy = 0;

                // Apply Sobel kernels for edge detection
                for (int m = -1; m <= 1; m++) {
                    for (int n = -1; n <= 1; n++) {
                        int pixel = imageData[(i + m) * (width * 3 + padding) + (j + n) * 3 + k];
                        gx += sobelX[(m + 1) * 3 + (n + 1)] * pixel;
                        gy += sobelY[(m + 1) * 3 + (n + 1)] * pixel;
                    }
                }

                // Calculate gradient magnitude (edge strength)
                int edge = sqrt(gx * gx + gy * gy);
                if (edge > maxEdge) {
                    maxEdge = edge;
                }
            }

            // Set the pixel to white if the edge strength exceeds the threshold, otherwise black
            unsigned char edgeValue = (maxEdge >= threshold) ? 255 : 0;

            for (int k = 0; k < 3; k++) {
                edgeData[i * (width * 3 + padding) + j * 3 + k] = edgeValue;
            }
        }
    }

    // Step 9: Write the edge data to the output file (using the same header)
    if (fwrite(imageHeader, sizeof(unsigned char), 54, out) != 54) {
        printf("Error writing image header\n");
        free(imageData);
        free(edgeData);
        return;
    }

    // Write the edge data
    for (unsigned int i = 0; i < height; i++) {
        if (fwrite(edgeData + i * (width * 3 + padding), sizeof(unsigned char), width * 3, out) != width * 3) {
            printf("Error writing edge data\n");
            free(imageData);
            free(edgeData);
            return;
        }
        for (unsigned int j = 0; j < padding; j++) {
            fputc(0, out);  // Handle padding in output file
        }
    }

    free(imageData);
    free(edgeData);
}