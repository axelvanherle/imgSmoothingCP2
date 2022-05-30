#include <stdio.h>
#include <stdlib.h>


#define BMPOUTPUT "outputImage.bmp"

void imageSmoothing (unsigned char *,unsigned char *,signed int,signed int,int);
void imageRed (unsigned char *,unsigned char *,signed int,signed int,int);
void imageGreen (unsigned char *,unsigned char *,signed int,signed int,int);
void imageBlue (unsigned char *,unsigned char *,signed int,signed int,int);
void imageBlack (unsigned char *,unsigned char *,signed int,signed int,int);
void chooseEffect (int,unsigned char *,unsigned char *,signed int,signed int,int);
/*
*   This functions edits the image and gets the smoothed image back.
*/



int main(int argc, char const *argv[])
{
    //Variable declaration of the input file.
	char BMPINPUT[20]; 
	printf("which file do you want to change?\n");
	scanf("%s",&BMPINPUT);
	
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
	
	int userChoiceEffect = 0;

   printf("\n=====================================\n");
	printf("Choose which effect you want to use.\n\n");
    printf("[1] - Smoothe the image.\n");
    printf("[2] - Apply a red filter to the image.\n");
    printf("[3] - Apply a green filter to  the image.\n");
    printf("[4] - Apply a blue filter to  the image.\n");
    printf("[5] - Apply a black and white filter to  the image.\n");
    fflush(stdin);
    printf("\nEnter your choice ==> ");
	scanf("%d",&userChoiceEffect);
	chooseEffect (userChoiceEffect,originalImagePixels,editedImagePixels,inputHeight,inputWidth,totalPixelsOriginal);
    

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


void chooseEffect (int userChoiceEffect,unsigned char * originalImagePixels,unsigned char * editedImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal ){
	
	switch(userChoiceEffect){
		case 1 :
		imageSmoothing(originalImagePixels,editedImagePixels,inputHeight,inputWidth,totalPixelsOriginal);
		break;
		
		case 2:
		imageRed(originalImagePixels,editedImagePixels,inputHeight,inputWidth,totalPixelsOriginal);
		break;
		
		case 3:
		imageGreen(originalImagePixels,editedImagePixels,inputHeight,inputWidth,totalPixelsOriginal);
		break;
		
		case 4:
		imageBlue(originalImagePixels,editedImagePixels,inputHeight,inputWidth,totalPixelsOriginal);
		break;
		
		case 5:
		imageBlack(originalImagePixels,editedImagePixels,inputHeight,inputWidth,totalPixelsOriginal);
		break;
		
		default:
		printf("error\n");
		break;
		
	}
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

void imageRed (unsigned char * originalImagePixels,unsigned char * editedImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal)
{   
    //These are used to store the values around the middle pixel (and the middle pixel itself.).
    int pixel0[3];
    

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
					if(i == 2 ){
                    pixel0[i] = originalImagePixels[(((y-1)*inputHeight+(x-1))*3)+i];
					}
					else{
						pixel0[i] = 0;
					}
                }
                for (int i = 0; i < 3; i++)
                {
                    editedImagePixels[((y*inputHeight+x)*3)+i] = pixel0[i];
                }
            }
        }
    }
}

void imageGreen (unsigned char * originalImagePixels,unsigned char * editedImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal)
{   
    //These are used to store the values around the middle pixel (and the middle pixel itself.).
    int pixel0[3];
    

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
					if(i == 1 ){
                    pixel0[i] = originalImagePixels[(((y-1)*inputHeight+(x-1))*3)+i];
					}
					else{
						pixel0[i] = 0;
					}
                }
                for (int i = 0; i < 3; i++)
                {
                    editedImagePixels[((y*inputHeight+x)*3)+i] = pixel0[i];
                }
            }
        }
    }
}

void imageBlue (unsigned char * originalImagePixels,unsigned char * editedImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal)
{   
    //These are used to store the values around the middle pixel (and the middle pixel itself.).
    int pixel0[3];

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
					if(i == 0 ){
                    pixel0[i] = originalImagePixels[(((y-1)*inputHeight+(x-1))*3)+i];
					}
					else{
						pixel0[i] = 0;
					}
                }
                for (int i = 0; i < 3; i++)
                {
                    editedImagePixels[((y*inputHeight+x)*3)+i] = pixel0[i];
                }
            }
        }
    }
}

void imageBlack (unsigned char * originalImagePixels,unsigned char * editedImagePixels,signed int inputHeight,signed int inputWidth,int totalPixelsOriginal)
{   
    //These are used to store the values around the middle pixel (and the middle pixel itself.).
    int pixel0[3];
	int counter = 0;

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
					if(originalImagePixels[(((y-1)*inputHeight+(x-1))*3)+i] <=127){
                    counter++;
					}
					
					
                }
				
				for (int i = 0; i < 3; i++){
					if(counter >= 2){
					pixel0[i] = 0;
					}
					else{
					pixel0[i] = 255;
					}
					if(i == 2){
						counter = 0;
					}
				}
				
				
                for (int i = 0; i < 3; i++)
                {
                    editedImagePixels[((y*inputHeight+x)*3)+i] = pixel0[i];
                }
            }
        }
    }
}
