// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

// ./volume input.wav output.wav 2.0
int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    // allocate memory for the header being read.
    uint8_t *header = malloc(HEADER_SIZE * sizeof(uint8_t));

    // Read the header from the source file into array
    if (fread(header, HEADER_SIZE, 1, input) < 1)
    {
        printf("Error reading header from source file.\n");

        fclose(input);
        fclose(output);
        free(header);
        return 2;
    }
    /*
    for (int i = 0; i < HEADER_SIZE; i++)
    {
        printf("%i\n", header[i]);
    }
    */

    if (fwrite(header, HEADER_SIZE, 1, output) < 1)
    {
        printf("Error writing header to output file.\n");
        free(header);

        fclose(input);
        fclose(output);
        return 3;
    }

    free(header);

    // TODO: Read samples from input file and write updated data to output file
    // buffer to get samples
    int16_t buffer;
    while (fread(&buffer, sizeof(int16_t), 1, input) > 0) // Get sizeof(int16_t) 1 at a time
    {
        buffer *= factor;
        // if successfully read, then scale it by `factor` and write it to the output file.
        if (fwrite(&buffer, sizeof(int16_t), 1, output) < 1)
        {
            printf("Error writing the sample to the output file.\n");
            fclose(input);
            fclose(output);
            return 4;
        }
    }
    // Close files
    fclose(input);
    fclose(output);
}
/*
Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!

check50 cs50/labs/2023/x/volume

Results for cs50/labs/2023/x/volume generated by check50 v3.3.8
:) volume.c exists
:) volume.c compiles
:) reduces audio volume, factor of 0.5 correctly
:) reduces audio volume, factor of 0.1 correctly
:) increases audio volume, factor of 2 correctly
--
Execute the below to evaluate the style of your code using style50.

style50 volume.c
*/