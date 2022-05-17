#include <stdio.h>
#include <stdlib.h>

#define BMPINPUT "inputImage.bmp"
#define BMPOUTPUT "outputImage.bmp"


int main(int argc, char const *argv[])
{
    //Variable declaration of the input file.
    FILE * inputBMP = fopen(BMPINPUT, "rb");

    signed int inputHeight = 0;
    signed int inputWidth = 0;
    int totalPixelsOriginal = 0;
    unsigned char * originalImagePixels = NULL;
    unsigned char inputHeader[54] = {0};

    //Variable declaration of the output file.
    FILE * outputBMP = fopen(BMPOUTPUT, "wb");

    //Checks if the input image is valid.
    if(inputBMP == NULL)
    {
        printf("%s\n", "ERROR: can't open the file");
        return -1;
    }
    //Checks if the output image is valid.
    if(outputBMP == NULL)
    {
        printf("%s\n", "ERROR: can't open the file");
        return -1;
    }

    //Gets width and height of the input image.
    inputWidth = inputHeader[21] << 24 | inputHeader[20] << 16 | inputHeader[19] << 8 | inputHeader[18];
    printf("The width of the input image: %d\n", inputWidth);
    inputHeight = inputHeader[25] << 24 | inputHeader[24] << 16 | inputHeader[23] << 8 | inputHeader[22];
    printf("The height of the input image: %d\n", inputHeight);
    printf("\n");

    //Calculates the size needed to save the input image into "originalImagePixels" and mallocs it.
    totalPixelsOriginal = inputWidth * inputHeight;
    originalImagePixels = (unsigned char *) malloc(totalPixelsOriginal*3);
    if(originalImagePixels == NULL)
    {
        printf("ERROR: memory allocation Failed\n");
        return -2;
    }

    //Gets the header from the input file and saves it into "inputHeader"
    fread(inputHeader, 1, 54, inputBMP);
    //Gets the pixels from the input file and saves it into "originalImagePixels"
    fread(originalImagePixels, 1, totalPixelsOriginal*3, inputBMP);
    printf("INFO: Heap memory allocated = %d (bytes)\n", totalPixelsOriginal*3);

    //Closes the input fine after inputHeader and originalImagePixels is copied into heap memory.
    fclose(inputBMP);
    printf("\n");
    printf("INFO: File %s CLOSED\n", BMPINPUT);

    //Writes inputHeader and originalImagePixels into the output image.
    fwrite(inputHeader,sizeof(char),sizeof(inputHeader),outputBMP);
    printf("Header copied.\n");
    fwrite(originalImagePixels,sizeof(char),totalPixelsOriginal*3,outputBMP);
    printf("Original image copied.\n");

    //Cleanup.
    fclose(outputBMP);
    printf("INFO: File %s CLOSED\n", BMPOUTPUT);
    free(originalImagePixels);
    printf("INFO: Heap memory Freed = %d (bytes)\n", totalPixelsOriginal*3);

    return 0;
}
