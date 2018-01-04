#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Program that will read a file and output its contents along with a conversion from hex to ascii like so
//everything after hex 20 is a character, 20 is space
//      21 22 23 24 25 26           !  "  #  $  %   

#define line_size 16


const char error[] = "Can not locate file\n";
const char help[] = "Enter the name of a file and have the contents printed into a new file with their ascii character counterparts\n";
const char smash[] = "Rename your file to something less stupid or literally go to hell\n";
const char txt[] = ".txt";

int main(int argc, char *argv[]){
    
    FILE *source, *dump;                                        //Source is file to be outputted to
    char dumpfile[100];                                         //name of new file to be outputted to
    char current[line_size];
    char c;
    int  done=0;
    int  i=0, L=-line_size, x;
    
    
    if (argc == 1){                                             //Running program with no arguments shows a help screen
        
        printf("%s", help);
        return 1;
    }
    
    if (strlen(argv[1]) > 100){                                 //A modest attempt to prevent stack smashing
        printf(smash);
        return 9;
    }
    
    strcpy(dumpfile, argv[1]);                                  //New file will be called FILENAME.EXT.txt
    strcat(dumpfile, ".txt");
    
    if(!(source = fopen(argv[1], "r"))){                        //Open the file given, if no file exists then throw error and exit
        printf("%s", error);
        return 1;
    }
    
    dump = fopen(dumpfile, "a");                                //Creates open file called FILENAME.txt
    if(dump == NULL){
        dump = fopen(dumpfile, "w");                            //If none exists, create an one
    }
    
    
    fseek(source, 0, SEEK_SET);                                 //Go to start of file
    
    fprintf(dump,"%s\n", argv[1]);
    //get into the array
    //check each variable of the array
    
    int final = 0;
    
    fprintf(dump, "                0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F              0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    //make a loop to print out each of the numbers rather than having this header string
    
    while(!done){                                               //Read 16 bytes at a time into an array and print out that array once in hex and once in ascii
        
        if (i == line_size){                                    //If the 16 bytes of memory have been read
            
            i = 0;                                              //Reset the byte counter
            fprintf(dump, "\n%08x   :   ", L & 0xffff);         //Print location in memory
            
            for(x=0; x<line_size; x++){
                fprintf(dump, "%02x   ", current[x] & 0xff);  //Print out each byte separated by two spaces
            }
            fprintf(dump ,"           ");                       //Buffer
            
            for(x=0; x<line_size; x++){                         
                if(current[x] > 31){                            //If the byte being read is an ascii character (hex value larger than 31)
                    fprintf(dump, "%c  ", current[x]);          //Print the actual character
                }
                else{                                           //Otherwise it has no visible character
                    fprintf(dump, ".  ");                       //Just print a period in its place
                }
            }
            
            if (final){done = 1;}                               //If we have reached the end of the file and do not need to print out anymore
        }
        c = fgetc(source);                                      //Read one byte into c
        if(c == EOF){final = 1;}                                //If we have read the end of the file then this is the last time we need to print anything out
            
        current[i] = c;                                         //put new byte into the data array
        
        i++;                                                    //Increment the byte counter
        L+=(line_size/16);                                      //Increment the memory address
    }
    
    
    
    fprintf(dump, "\n\n");
    fclose(dump);                                               //Close everything and exit
    fclose(source);
    return 0;
}
