/*
 * Somewhat edited file that resizes a bitmap image.
 * Probably not the most efficient but anything that
 * gets me closer to finishing low level memory is a plus.
 */
 
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    BITMAPFILEHEADER out_bf = bf;
    BITMAPINFOHEADER out_bi = bi;
    
    out_bi.biWidth *= n;
    out_bi.biHeight *= n;
    int inPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outPadding = (4 - (out_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    out_bi.biSizeImage = ((out_bi.biWidth * sizeof(RGBTRIPLE)) + outPadding) * abs(out_bi.biHeight);
    out_bf.bfSize = out_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // write each outfile scanline n-1 times in rows
        for (int k = 0; k < n - 1; k++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile n times
                for (int q = 0; q < n; q++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // add padding
            for (int m = 0; m < outPadding; m++)
            {
                fputc(0x00, outptr);
            }
            
            fseek(inptr, 54 + (bi.biWidth * 3 + inPadding) * i, SEEK_SET);
        }
        
        // iterate over pixels in final scanline
        for (int l = 0; l < bi.biWidth; l++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for (int r = 0; r < n; r++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // then add it back (to demonstrate how)
        for (int p = 0; p < outPadding; p++)
        {
            fputc(0x00, outptr);
        }
        
        // skip over padding, if any
        fseek(inptr, inPadding, SEEK_CUR);
       
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
