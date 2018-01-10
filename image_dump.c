#include <stdio.h>
#include <stdlib.h>

const char error[] = "Can not locate file";
const char help[] = "Enter the name of a file and have the contents printed in hex\n";
const char option_f[] = "-f";

//Don't use this anymore, use the hexdump.c program instead

int main(int argc, char *argv[]){
    
    int line_break = 54; //Number of outputs before line break
    FILE *image, *dump;
    int c;
    int i = 1, x = 1;
    int start_point, width, height;
    
    if (argc == 1){
        
        printf("%s", help);
        return 0;
    }

    
    image = fopen(argv[1], "r");
    dump = fopen("dump.txt", "a");
    if (image) {
        
        
        fseek(image, 0, SEEK_SET);//Back to the start
        
        //Only feed images that are less than 256 X 256
        fseek(image, 10 , 0); //get width
        start_point = fgetc(image);
        
        fseek(image, 18 , 0); //get width
        width = fgetc(image);
        
        fseek(image, 22 , 0); //get height
        height = fgetc(image);
        
        fseek(image, 0,0);
        
        fprintf(dump, "Width : %d, Height : %d\n\n", width, height);
        for(x=0; x < line_break; x++){
            c = getc(image);
            fprintf(dump, "%hhX ", c);
        }
        
        fprintf(dump, "\n\n");
        
        while ((c = getc(image)) != EOF){
            fprintf(dump, "%hhX ", c);
            i++;
            
            if(i == width*3){
                fprintf(dump, "\n");
                i = 1;
            }
        }
        fclose(image);
        fclose(dump);
    }else{
        printf("%s", error);
    }
    
    printf("\n");
}
