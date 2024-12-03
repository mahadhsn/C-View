
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define BMP header structures
#pragma pack(push, 1) // Ensure no padding
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
#pragma pack(pop)

void reflection(const char *inputFile, const char *outputFile) {
    FILE *in = fopen(inputFile, "rb");
    if (!in) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *out = fopen(outputFile, "wb");
    if (!out) {
        fclose(in);
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    // Read BMP headers
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, in);
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, in);

    // Validate BMP format
    if (fileHeader.bfType != 0x4D42) {
        fprintf(stderr, "Unsupported file format.\n");
        fclose(in);
        fclose(out);
        exit(EXIT_FAILURE);
    }

    // Write headers to the output file
    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, out);

    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    int padding = (4 - (width * 3) % 4) % 4;

    // Allocate memory for one row of pixel data
    uint8_t *row = malloc(width * 3);

    for (int i = 0; i < height; i++) {
        fread(row, 3, width, in);          // Read a row
        fseek(in, padding, SEEK_CUR);     // Skip padding

        // Reflect the row horizontally
        for (int j = 0; j < width / 2; j++) {
            for (int k = 0; k < 3; k++) { // Swap RGB values
                uint8_t temp = row[j * 3 + k];
                row[j * 3 + k] = row[(width - 1 - j) * 3 + k];
                row[(width - 1 - j) * 3 + k] = temp;
            }
        }

        fwrite(row, 3, width, out);       // Write the row
        for (int p = 0; p < padding; p++) fputc(0, out); // Write padding
    }

    free(row);
    fclose(in);
    fclose(out);
}

