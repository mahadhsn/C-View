#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grayscale.h"
#include "reflection.h"
#include "blur.h"
#include "edgeDetect.h"
#include "rotation.h"

//help files
void help() {
    printf("c-view: v2.1.0\n");
    printf("c-view is a utility to perform operations and add filters onto image files.\n\n");
    printf("Usage: ./c-view [OPTIONS]\n");
    printf("Options:\n");
    printf("  --help           Display this help message and exit\n");
    printf("  -gs              flag will convert the image into a grayscaled version\n");
    printf("  -rs              flag will convert the image into a reflected version according to input\n");
    printf("  -rot             flag will convert the image into a rotated version according to input\n");
    printf("  -bl              flag will convert the image into a blurred version\n");
    printf("  -ed              flag will convert the image into a version with edges highlighted\n");
    printf("Examples:\n");
    printf("  ./c-view -gs [image file location] [image file output]\n");
    exit(0);
}

void usage() {
    printf("Usage: ./c-view [OPTIONS]\n");
    printf("Try './c-view --help' for more information.\n");

}

int main(int argc, char* argv[]){
    unsigned char imageHeader[54];
    if (argc == 4 || argc == 2) {
        if(strcmp(argv[1], "--help") == 0){
            help();
            return 0;
        } 

        //printf("Opening input file: %s\n", argv[2]);
        FILE *in = fopen(argv[2], "rb");
        if(!in){
            //printf("Error opening input file\n");
            usage();
            return 1;
        }

        //printf("Input file opened successfully: %s\n", argv[2]);

        if(strcmp(argv[1], "-gs") == 0){
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                //printf("Error opening output file\n");
                fclose(in);
                return 1;
            }
            grayscale(in, out);
            fclose(out);
        }else if(strcmp(argv[1], "-rs") == 0){
            reflection(argv[2], argv[3]);
            
        }else if (strcmp(argv[1], "-rot") == 0) {
            //printf("Rotation option selected\n");
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                //printf("Error opening output file\n");
                perror("fopen");
                fclose(in);
                return 1;
            }
            rotateLeft90(in, out);
            fclose(out);

        }else if(strcmp(argv[1], "-bl") == 0){
            blur();
        }else if(strcmp(argv[1], "-ed") == 0){
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                printf("Error opening output file\n");
                fclose(in);
                return 1;
            }
            edgeDetect(in, out);
            fclose(out);

        }else{
            //printf("Invalid flag\n");
            usage();
            return 1;
        }
        fclose(in);
    }else{
        //printf("Invalid number of arguments\n");
        usage();
        return 1;
    }
    
    return 0;
}