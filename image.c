#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//https://en.wikipedia.org/wiki/BMP_file_format

#define max 3*(256*256)-1 //Maximum filename size
const char no_file[] = "File does not exist";
const char error[] = "Invalid arguments, run file with no arguments for help\n";
const char help[] = "./image <filename>\n";
const char too_big[] = "Image is too large or is corupted, keep filesize under 64k por favor\n";
const char wrong_bit[] = "This program only supports 24-bit bitmap as of now\n";

/*Returns
    0 : Sucess
    1 : You done fucked up
    2 : Overflow probably
*/

/*Colors
    0 :  (space)
    1 : *
    2 : #
    3 : $
*/


int main(int argc, char *argv[]){

    FILE *image, *dump;
    signed int width, height;
    int c;
    int i = 0, f = 0, a = 54, x;
	char *buffer; // Buffer for the hex to dec
	int image_size;
	char txt[] = "txt";
	int avg;
	
	unsigned char red, green, blue;
    
	
    if (argc == 1){
        printf("%s", help);
        return 1;
    }
    
    if (argc > 2){
        printf("%s", error);
        return 1;
    }
    
    
    image = fopen(argv[1], "r");
    
    dump = fopen("ascii.txt", "a");
    if(dump == NULL){
        dump = fopen("ascii.txt", "w");
    }
    fprintf(dump, "\n\n%s\n", argv[1]);
    
    //just add this ascii art to the end of the file and put the filename in front of it
    
    if(image){
        
        fseek(image, 0, SEEK_END);
        image_size = ftell(image);
        
        if(image_size > max){
            printf("%s", too_big);
            return 2;
        }
        
        //buffer = (char *) malloc(image_size); //gets filesize and allocates memory for it
        
        fseek(image, 0, SEEK_SET);//Back to the start
        
        //Only feed images that are less than 256 X 256
        fseek(image, 18 , 0); //get width
        width = fgetc(image);
        
        fseek(image, 22 , 0); //get height
        height = fgetc(image);
        
        fseek(image, 28 , 0); //check that this is a 24 bit bitmap
        
        if(fgetc(image) != 24){
            printf("%s", wrong_bit);
            return 2;
        } 
    
        printf("Image Size -> %d kbs, Width -> %d pxl, Height -> %d pxl\n", image_size/1024, width, height);
        //For the sake of saving time, just assume headers are all 54 bytes long
        
        fseek(image, a, 0);
        
        for(x=0; x < height*width; x++){                 //(c = getc(image)) != EOF){
            red = fgetc(image);
            blue = fgetc(image);
            green = fgetc(image);
            
            //printf("[%d,%d]  R : %d, G : %d, B : %d\n", f, i, red, blue, green);
            
            avg = ((int)red + (int)blue-1 + (int)green)/3;
            
            if(avg < 51){
                fprintf(dump, "#");
            }else if((avg >= 51) && (avg < 102)){
                fprintf(dump, "*");
            }else if((avg >= 102) && (avg < 153)){
                fprintf(dump, "^");
            }else if((avg >= 153) && (avg < 204)){
                fprintf(dump, "-");
            }else{
                fprintf(dump, " ");
            }

            if(i == width-1){
                i = 0;
                f++;
                fprintf(dump, " BR \n");
            }else{
                i++;
            }
        }
    
    
        //after everything is done
        free(buffer);
        printf("\n");
        fclose(image);
        fclose(dump);
        return 0;
    }else{
        printf("%s\n",no_file);
        return 1;
    }
}
