// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember scale factor and filenames
    int scalefactor = atoi(argv[1]);
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

    int original_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int original_width = bi.biWidth;
    int original_height = bi.biHeight;


    int new_width = round((bi.biWidth) * scalefactor);
    int new_height = round((bi.biHeight) * scalefactor);

    bi.biHeight = new_height;
    bi.biWidth = new_width;

    // determine new_padding for scanlines
    int new_padding = (4 - (new_width * sizeof(RGBTRIPLE)) % 4) % 4;

    printf("new_padding is %i\n", new_padding);

    printf("Original bitmap file size is %i\n", bf.bfSize);

    bf.bfSize = 54 + ((abs(new_height) * (abs(new_width))) * sizeof(RGBTRIPLE)) + (new_padding * abs(new_height));
    bi.biSizeImage = abs((new_height) * (abs(new_width))) * sizeof(RGBTRIPLE) + (new_padding * abs(new_height));

    printf("Scaled bitmap file size is %i\n", bf.bfSize);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    int loop_counter = fabs(original_width * scalefactor * scalefactor);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(original_height); i < biHeight; i++)
    {

        for (int z = 0; z < scalefactor; z++)
        {


            // iterate over pixels in scanline
            for (int j = 0; j < original_width; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                printf("i: %i -- z: %i -- j: %i -- inptr located at %ld\n", i, z, j, ftell(inptr));
                for (int k = 0; k < scalefactor; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

                }


            }

            // skip over padding, if any
            fseek(inptr, original_padding, SEEK_CUR);

            //Go back to scan line
            fseek(inptr, -(original_width  * sizeof(RGBTRIPLE)) - original_padding, SEEK_CUR);


            // skip over padding, if any
            if (ftell(outptr) + new_padding < bf.bfSize)
            {
                fseek(outptr, new_padding, SEEK_CUR);
            }


            printf("outptr located at %ld\n", ftell(outptr));
            // then add it back (to demonstrate how)

            for (int k = 0; k < new_padding; k++)
            {

                    printf("k: %i -- outptr writing to %ld\n", k, ftell(outptr));
                    fputc(0x00, outptr);

            }

            fseek(outptr, -new_padding, SEEK_CUR);

        }

         //After copying one row n times, go to start of next row
        fseek(inptr, +(original_width  * sizeof(RGBTRIPLE)) + original_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);
    fseek(outptr, 0, SEEK_END);
    printf("FINAL: outptr saving at %ld\n", ftell(outptr));
    // close outfile
    fclose(outptr);

    // success
    return 0;
}
