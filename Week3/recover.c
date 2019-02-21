#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

int main(int argc, char *argv[])
{

    //Only one argument
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    //Create buffer for data to be stored in
    uint8_t buffer [512];

    //Open file
    FILE *inptr = fopen(argv[1], "r");
    FILE *outptr;

    //Create variables to be used for file names
    int file_number = 0;
    char jpg_name[8];
    int jpeg_file_found = 0;
    jpg_name[7] = '\0';

    //Read until end of file
    while (fread(&buffer, 512, 1, inptr) != 0)
    {

        //JPEG string identifier
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0) //First four bits of the last byte equal 1110 (0xe0)

        {
            if (file_number > 0)
            {
                //Close out the file
                fclose(outptr);
            }
            //Assign name
            sprintf(jpg_name, "%03d.jpg", file_number);
            printf("%s\n", jpg_name);

            //Create file
            outptr = fopen(jpg_name, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", jpg_name);
                return 3;
            }

            //No need for fread since it is incorporated into the while loop
            fwrite(&buffer, 512, 1, outptr);
            file_number++;

        }
        else if (file_number > 0)

        {
            //No need for fread since it is incorporated into the while loop
            fwrite(&buffer, 512, 1, outptr);

        }

    }

    printf("Buffer read %i times.\n", file_number);
}