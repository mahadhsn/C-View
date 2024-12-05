#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grayscale.h"
#include "reflection.h"
#include "blur.h"
#include "edgeDetect.h"
#include "rotation.h"

////////////////////////////////////////
// Function to display help menu
// this function is called when the user inputs --help
// arguments: none
// returns: none
////////////////////////////////////////

void help() {
    printf("c-view: v2.2.0\n");
        
    printf(" ---------------------------------------\n");
    printf("|                c-view                 |\n");
    printf("|           Lightweight Image           |\n");
    printf("|            Processing Tool            |\n");
    printf(" ---------------------------------------\n\n");

    printf("Welcome to C-View! Here's what you can do:\n\n");
    
    printf(" ---------------------------------------\n");
    printf("| Options - Usage: ./c-view [OPTIONS]   |\n");
    printf("|---------------------------------------|\n");
    printf("| --help        Show this menu          |\n");
    printf("| -gs           Grayscale an image      |\n");
    printf("| -rs           Reflect an image        |\n");
    printf("| -rot          Rotate an image         |\n");
    printf("| -bl           Blur an image           |\n");
    printf("| -ed           Detect edges in an image|\n");
    printf(" ---------------------------------------\n");
    
    printf("\nExamples:\n");
    printf("  ./c-view -gs [image file location] [image file output]\n");
    exit(0);
}

////////////////////////////////////////
// Function to display usage menu
// this function is called when the user inputs invalid arguments
// arguments: none
// returns: none
////////////////////////////////////////

void usage() {
    printf("Usage: ./c-view [OPTIONS]\n");
    printf("Try './c-view --help' for more information.\n");

}

////////////////////////////////////////
// Main function
// this function is called when the program is run
// arguments: int argc, char* argv[]
// returns: int based on success
////////////////////////////////////////

int main(int argc, char* argv[]){
    unsigned char imageHeader[54];
    if (argc == 4 || argc == 2) {
        if(strcmp(argv[1], "--help") == 0){
            help();
            return 0;
        } 

        FILE *in = fopen(argv[2], "rb");
        if(!in){
            usage();
            return 1;
        }


        if(strcmp(argv[1], "-gs") == 0){
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                usage();
                fclose(in);
                return 1;
            }
            grayscale(in, out);
            fclose(out);
        }else if(strcmp(argv[1], "-rs") == 0){
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                usage();
                fclose(in);
                return 1;
            }
            reflection(in, out);
            fclose(out);
            
        }else if (strcmp(argv[1], "-rot") == 0) {
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                perror("fopen");
                fclose(in);
                return 1;
            }
            rotateLeft90(in, out);
            fclose(out);

        }else if(strcmp(argv[1], "-bl") == 0){
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                perror("fopen");
                fclose(in);
                return 1;
            }
		blur(in, out);
		fclose(out);

        }else if(strcmp(argv[1], "-ed") == 0){
            FILE *out = fopen(argv[3], "wb");
            if (!out) {
                perror("fopen");
                fclose(in);
                return 1;
            }
            edgeDetect(in, out);
            fclose(out);

        }else{
            usage();
            return 1;
        }
        fclose(in);
    }else{
        usage();
        return 1;
    }
    
    return 0;
}
