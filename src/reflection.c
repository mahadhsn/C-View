
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define BMP header structures which can be used to read the header of a BMP file
#pragma pack(push, 1) // Ensure no padding in memory of the following structures
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop) // Restore default padding


////////////////////////////////////////
// Function to reflect an image horizontally
// arguments: FILE *in, FILE *out the input and output files
// returns: none
////////////////////////////////////////

void reflection(FILE *in, FILE *out) {
    unsigned char header[54];

    fread(header, sizeof(unsigned char), 54, in);
    fwrite(header, sizeof(unsigned char), 54, out);

    int width = *(int *)&header[18];
    int height = *(int *)&header[22];

    int row_padded = (width * 3 + 3) & (~3);
    unsigned char *row_data = (unsigned char *)malloc(row_padded);

    if (!row_data) {
        printf("Error allocating memory\n");
        return;
    }

    for (int i = 0; i < height; i++) {
        fread(row_data, sizeof(unsigned char), row_padded, in);

        // Reflect row by swapping individual RGB values
        for (int j = 0; j < width / 2; j++) {
            int left_idx = j * 3;
            int right_idx = (width - j - 1) * 3;

            for (int k = 0; k < 3; k++) {
                unsigned char temp = row_data[left_idx + k];
                row_data[left_idx + k] = row_data[right_idx + k];
                row_data[right_idx + k] = temp;
            }
        }

        fwrite(row_data, sizeof(unsigned char), row_padded, out);
    }

    free(row_data);
    //printf("Reflection completed successfully\n");
}




