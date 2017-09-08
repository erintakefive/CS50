#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>

/*
 * For the most part, I actually made this myself. Except for the do/while loop
 * and the function declared below. This was done by https://github.com/endiliey/
 */

bool jpegfound(unsigned char buffer[]);

int main(int argc, char* argv[])
{
    // ensure correct usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage ./recover image\n");
        return 1;
    }
    
    // open file specified at command line
    FILE *inptr = fopen(argv[1], "r");
    
    // check file pointer for NULL
    if (inptr == NULL)
    {
        fprintf(stderr, "Unable to open image\n");
        return 2;
    }
    
    // initialise variables
    unsigned char buffer[512];
    int jpeg_number = 0;
    char filename[8];
    
    fread(&buffer, 512, 1, inptr);
    
    // repeat until EOF
    while (fread(&buffer, 512, 1, inptr) == 1)
    {
        // determine if jpeg, and write if so.
        if (jpegfound(buffer))
        {
            // assign new string to filename
            sprintf(filename, "%03i.jpg", jpeg_number);
                
            // create and open new jpeg file
            FILE* img = fopen(filename, "a");
            
            // increment jpeg_number for correct filename formatting
            jpeg_number++;
                
            // Write the jpeg when it's not a new jpeg file
            do
            {
                fwrite(&buffer, 512, 1, img);
                fread(&buffer, 512, 1, inptr);
            }
            while (!(jpegfound(buffer)) && feof(inptr) == 0);
            
            fclose(img);
            // move file indicator position back by 1 block (512 bytes)
            fseek(inptr, -1 * 512, SEEK_CUR);
        }
    }
    
    // close files
    fclose(inptr);
    return 0;
}

// alternative to malfunctioning bitwise stuff suggested by zamyla. thanks to https://github.com/endiliey/cs50
bool jpegfound(unsigned char buffer[])
{   
    if (buffer[0] != 0xff)
    {
        return false;
    }
        
    if (buffer[1] != 0xd8)
    {
        return false;
    }
        
    if (buffer[2] != 0xff)
    {
        return false;
    }
        
    switch (buffer[3])
    {
        case 0xe0:
            case 0xe1:
                case 0xe2:
                    case 0xe3:
                        case 0xe4:
                            case 0xe5:
                                case 0xe6:
                                    case 0xe7:
                                        case 0xe8:
                                            case 0xe9:
                                                case 0xea:
                                                    case 0xeb:
                                                        case 0xec:
                                                            case 0xed:
                                                                case 0xee:
                                                                    case 0xef: 
                                                                        break;
        default:
            return false;
    }
    return true;
}
