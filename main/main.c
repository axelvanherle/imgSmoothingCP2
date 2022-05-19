#include <stdio.h>
#include <stdlib.h>

#define BMPINPUT "inputImageBig.bmp"
#define BMPOUTPUT "outputImage.bmp"

void imageSmoothing (unsigned char *,unsigned char *,signed int,signed int,int);
/*
*   This functions edits the image and gets the smoothed image back.
*/

void weirdEffect (unsigned char * originalImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal);

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
    unsigned char * editedImagePixels = NULL;
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

    //Gets the header from the input file and saves it into "inputHeader"
    fread(inputHeader, 1, 54, inputBMP);

    //Gets width and height of the input image.
    inputWidth = inputHeader[21] << 24 | inputHeader[20] << 16 | inputHeader[19] << 8 | inputHeader[18]; 
    printf("The width of the input image: %d\n", inputWidth);
    inputHeight = inputHeader[25] << 24 | inputHeader[24] << 16 | inputHeader[23] << 8 | inputHeader[22]; 
    printf("The height of the input image: %d\n", inputHeight);
    printf("\n");

    //Calculates the size needed to save the input image into "originalImagePixels" and "editedImagePixels" and mallocs it.
    totalPixelsOriginal = inputWidth * inputHeight;

    originalImagePixels = (unsigned char *) malloc(totalPixelsOriginal*3);
    if(originalImagePixels == NULL)
    {
        printf("ERROR: memory allocation Failed original\n");
        return -2;
    }
    editedImagePixels = (unsigned char *) malloc(totalPixelsOriginal*3);
    if(editedImagePixels == NULL)
    {
        printf("ERROR: memory allocation Failed edited\n");
        return -2;
    }

    //Gets the pixels from the input file and saves it into "originalImagePixels"
    fread(originalImagePixels, 1, totalPixelsOriginal*3, inputBMP);
    printf("INFO: Heap memory allocated = %d (bytes)\n", totalPixelsOriginal*3);
    
    //Closes the input fine after inputHeader and originalImagePixels is copied into heap memory.
    fclose(inputBMP);   
    printf("\n");
    printf("INFO: File %s CLOSED\n", BMPINPUT);

    /*
    *   This functions edits the image and gets the smoothed image back.
    */
    imageSmoothing(originalImagePixels,editedImagePixels,inputHeight,inputWidth,totalPixelsOriginal);

    //Writes inputHeader and originalImagePixels into the output image.
    fwrite(inputHeader,sizeof(char),sizeof(inputHeader),outputBMP);
    printf("Header copied.\n");
    fwrite(editedImagePixels,sizeof(char),totalPixelsOriginal*3,outputBMP);
    printf("Original image copied.\n");

    /*
    * Cleanup.
    * Closes output and frees memory.
    */
    fclose(outputBMP);
    printf("INFO: File %s CLOSED\n", BMPOUTPUT);
    free(originalImagePixels);
    free(editedImagePixels);
    printf("INFO: Heap memory Freed = %d (bytes)\n", totalPixelsOriginal*3);

    printf("\n\n\n--------------------\n");
    printf("Image processing done.");

    return 0;
}

void imageSmoothing (unsigned char * originalImagePixels,unsigned char * editedImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal)
{   
    //These are used to store the values around the middle pixel (and the middle pixel itself.).
    int pixel0[3];
    int pixel1[3];
    int pixel2[3];
    int pixel3[3];
    int pixel4[3];
    int pixel5[3];
    int pixel6[3];
    int pixel7[3];
    int pixel8[3];

    //For loop to itterate trough the height of the image.
    for (int y = 0; inputHeight> y; y++)
    {   
        //For loop to itterate trough the width of the image.
        for (int x = 0; inputWidth>x; x++)
        {   
            //Checks if we are on the edge of the image.
            if(x == 0 || x == inputWidth-1 || y == 0 || y == inputHeight-1) 
            {
                for (int i = 0; i < 3; i++)
                {   
                    //If we are on the edge of the image, do this.
                    editedImagePixels[((y*inputHeight+x)*3)+i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                }
                
            }
            //If we arent on the edge of the image, do this.
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    pixel0[i] = originalImagePixels[(((y-1)*inputHeight+(x-1))*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel1[i] = originalImagePixels[(((y-1)*inputHeight+x)*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel2[i] = originalImagePixels[(((y-1)*inputHeight+(x+1))*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel3[i] = originalImagePixels[((y*inputHeight+(x-1))*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel4[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel5[i] = originalImagePixels[((y*inputHeight+(x+1))*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel6[i] = originalImagePixels[(((y+1)*inputHeight+(x-1))*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel7[i] = originalImagePixels[(((y+1)*inputHeight+x)*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    pixel8[i] = originalImagePixels[(((y+1)*inputHeight+(x+1))*3)+i];
                }
                for (int i = 0; i < 3; i++)
                {
                    editedImagePixels[((y*inputHeight+x)*3)+i] = (pixel0[i]+pixel1[i]+pixel2[i]+pixel3[i]+pixel4[i]+pixel5[i]+pixel6[i]+pixel7[i]+pixel8[i]) / 9;
                }
            }
        }
    }
}

void weirdEffect (unsigned char * originalImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal)
{   
    //STILL A WIP
    int pixel0[2];
    int pixel1[2];
    int pixel2[2];
    int pixel3[2];
    int pixel4[2];
    int pixel5[2];
    int pixel6[2];
    int pixel7[2];
    int pixel8[2];

    //For loop to itterate trough the height of the image.
    for (int y = 0; inputHeight> y; y++)
    {   
        //Checks if the image is either in the first or last line of height.
        if(y == 0 || y == inputHeight-1) 
        {
            //Do nothing, we ingore the first and last line.
        }
        //If we arent on the first or last line do this.
        else
        {
            for (int x = 0; inputWidth>x; x++)
            {   
                if(x == 0 || x == inputWidth-1) 
                {
                    //Do nothing, we ingore the first and last line.
                }
                //If we arent on the first or last line do this.
                else
                {
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel0[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel1[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel2[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel3[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel4[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel5[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel6[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel7[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        pixel8[i] = originalImagePixels[((y*inputHeight+x)*3)+i];
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        //printf("PIXEL[%d] = %x\n",((y*inputHeight+x)*3)+i,originalImagePixels[((y*inputHeight+x)*3)+i]);
                        originalImagePixels[((y*inputHeight+x)*3)+i] = (pixel0[i]+pixel1[i]+pixel2[i]+pixel3[i]+pixel4[i]+pixel5[i]+pixel6[i]+pixel7[i]+pixel8[i]) / 9;
                    }
                }
            }
        }
    }
}