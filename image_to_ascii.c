#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//https://en.wikipedia.org/wiki/BMP_file_format

#define max (3*256*256)-1 //Maximum file size
const char no_file[] = "File does not exist\n";
const char error[] = "Invalid arguments, run file with no arguments for help\n";
const char help[] = "./image <filename>\n";
const char too_big[] = "Image is too large or is corupted, keep filesize under 196k or 256*256 por favor\n";
const char wrong_bit[] = "This program only supports 24-bit bitmap (no alpha channel)\n";

/*Returns
    0 : Sucess
    1 : Argument Error
    2 : Image is too large
    3 : Image is not 24 bit
*/


int main(int argc, char *argv[]){

    FILE *image, *dump;
    signed int width, height;
    int i = 0;                                              //Used to determine padding
    int start_point = 0;
    int x;                                                  //Counter Variable
    int m;                                                  //Counter for offset
    int h_correction;                                       //Each line must be a multiple of 4, this is the amount of padding used to acchive that
	int image_size;
	int avg;
	
	unsigned char red, green, blue;
    
	
    if (argc == 1){                                         //If no file given print a help text
        printf("%s", help);
        return 1;
    }
    
    if (argc > 2){                                          //If more than one argument is given print an error
        printf("%s", error);
        return 1;
    }
    
    
    image = fopen(argv[1], "r");                            //Open the File
    
    dump = fopen("ascii.txt", "a");                         //Open the Dump file
    if(dump == NULL){
        dump = fopen("ascii.txt", "w");                     //If none exists, create an ascii.txt
    }
    
    if(image){
        
        fseek(image, 0, SEEK_END);                          //Get size of file
        image_size = ftell(image);
        
        if(image_size > max){                               //If image is larger than max value then abort
            printf("%s", too_big);
            fclose(image);
            fclose(dump);
            return 2;
        }
        
        fseek(image, 0, SEEK_SET);                          //Back to the start of file
        
        fseek(image, 10 , 0);                               //Get location of start of image data
        start_point = fgetc(image);
        
        fseek(image, 18 , 0);                               //image width
        width = fgetc(image);
        h_correction = (width%4);                           //determine the padding at the end of each line
        
        fseek(image, 22 , 0);                               //get height
        height = fgetc(image);
        
        if((width > 256) || (height > 256)){                //If image is larger than expected then abort
            printf("%s", too_big);
            fclose(image);
            fclose(dump);
            return 2;
        }
        
        fseek(image, 28 , 0);                               //check that this is a 24 bit bitmap
        
        if(fgetc(image) != 24){                             //If not then abort
            printf("%s", wrong_bit);
            return 3;
        } 
        
        fprintf(dump, "\n\n%s\n", argv[1]);                 //Print name of file before the image dump
        
        printf("Image Size -> %d bytes, Width -> %d pxl, Height -> %d pxl, Correction -> %d\n", image_size, width, height, h_correction);
        
        //fseek(image, 0, SEEK_SET);//Back to the start
        fseek(image, start_point, 0);
        
        for(x=0; x < height*width; x++){
            
            red = fgetc(image);
            blue = fgetc(image);
            green = fgetc(image);
            
            avg = ((int)red + (int)blue-1 + (int)green)/3;
            
            if(avg < 32){                               //Darker colors are lower average numbers
                fprintf(dump, "#");
            }else if((avg >= 32) && (avg < 54)){
                fprintf(dump, "X");
            }else if((avg >= 54) && (avg < 72)){
                fprintf(dump, "I");
            }else if((avg >= 72) && (avg < 96)){
                fprintf(dump, "*");
            }else if((avg >= 96) && (avg < 128)){
                fprintf(dump, "i");
            }else if((avg >= 128) && (avg < 150)){
                fprintf(dump, ";");
            }else if((avg >= 150) && (avg < 187)){
                fprintf(dump, "^");
            }else if((avg >= 187) && (avg < 210)){
                fprintf(dump, ".");
            }else{
                fprintf(dump, " ");
            }
            
            

            if(i == width-1){                           //If you have read all color data from a line, i keeps track of this
                i = 0;                                  //Some padding may still remain
                fprintf(dump, "\n");                    //Print new line
                for(m=0; m < h_correction; m++){        //skip the padding
                    fgetc(image);
                }
            }else{
                i++;                                    //we arent at the end of a line
            }
        }
    
    
                                                        //after everything is done
        printf("\n");
        fclose(image);
        fclose(dump);
        return 0;
    }else{                                              //If the file never opens to begin with 
        printf("%s\n",no_file);
        fclose(dump);
        return 1;
    }
}
