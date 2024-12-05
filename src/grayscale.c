#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////
// Function to convert an image to grayscale
// arguments: FILE *in, FILE *out the input and output files
// returns: none
////////////////////////////////////////

void grayscale(FILE *in, FILE *out) {
    unsigned char header[54];
    
    fread(header, sizeof(unsigned char), 54, in); 
    fwrite(header, sizeof(unsigned char), 54, out);

    int width = *(int *)&header[18];
    int height = *(int *)&header[22];

    int row_padded = (width * 3 + 3) & (~3); 

    unsigned char *pixel_data = (unsigned char *)malloc(row_padded);

    for (int i = 0; i < height; i++) {
        fread(pixel_data, sizeof(unsigned char), row_padded, in); 
        for (int j = 0; j < width * 3; j += 3) {
            unsigned char blue = pixel_data[j];
            unsigned char green = pixel_data[j + 1];
            unsigned char red = pixel_data[j + 2];

            unsigned char gray = (unsigned char)(0.299 * red + 0.587 * green + 0.114 * blue);

            pixel_data[j] = gray;
            pixel_data[j + 1] = gray;
            pixel_data[j + 2] = gray;
        }
        fwrite(pixel_data, sizeof(unsigned char), row_padded, out);
    }

    free(pixel_data); 
}