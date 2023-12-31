#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

#define WAVHEADERSIZE 44

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

// ./reverse input.wav output.wav
int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse [input.wav] [output.wav]n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Cannot open input file.\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER *inputWavHeader = malloc(sizeof(WAVHEADER));
    if (fread(inputWavHeader, sizeof(WAVHEADER), 1, input) != 1)
    {
        free(inputWavHeader);
        printf("Cannot read header.\n");
        return 1;
    }

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(*inputWavHeader) != 0)
    {
        free(inputWavHeader);
        fclose(input);
        printf("Input file is not WAV format.\n");
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        free(inputWavHeader);
        fclose(input);
        printf("Cannot create output file.\n");
        return 1;
    }

    // Write header to file
    // TODO #6
    if (fwrite(inputWavHeader, sizeof(WAVHEADER), 1, output) != 1) // write sizeof(WAVHEADER) 1 time to output
    {
        free(inputWavHeader);
        fclose(input);
        fclose(output);
        printf("Cannot write to output file.\n");
        return 1;
    }

    // Use get_block_size to calculate size of block
    // TODO #7
    int blockSize = get_block_size(*inputWavHeader);

    // Write reversed audio to file
    // TODO #8
    BYTE blockInfo[blockSize];
    long endOfHeaderPos = ftell(input);

    if (fseek(input, 0, SEEK_END) != 0) // go to the end.
    {
        free(inputWavHeader);
        fclose(input);
        fclose(output);
        printf("fseek 1 error.\n");
        return 1;
    }

    // Jump back 2 chunks since after a blockSize is read the pointer will be at end of the block, go back 2 to start at a new one.
    while (endOfHeaderPos < ftell(input))
    {

        if (fseek(input, 0 - blockSize, SEEK_CUR) != 0)
        {
            free(inputWavHeader);
            fclose(input);
            fclose(output);
            printf("fseek 2 error.\n");
            return 1;
        }

        if (fread(blockInfo, blockSize, 1, input) != 1) // read blockSize bytes 1 time from input
        {
            free(inputWavHeader);
            fclose(input);
            fclose(output);
            printf("Cannot read block size from input.\n");
            return 1;
        }

        if (fwrite(blockInfo, blockSize, 1, output) != 1) // write blockSize bytes 1 time to output
        {
            free(inputWavHeader);
            fclose(input);
            fclose(output);
            printf("Cannot write block size to output.\n");
            return 1;
        }
        // since fread will move the pos to the end of the block, move it back to the beginning so the next loop has the correct
        // start pos to go back again.
        if (fseek(input, 0 - blockSize, SEEK_CUR) != 0)
        {
            free(inputWavHeader);
            fclose(input);
            fclose(output);
            printf("fseek 3 error.\n");
            return 1;
        }
    }

    fclose(input);
    fclose(output);

    free(inputWavHeader);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 0; // true
    }
    return 1; // false
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    // blockSize = number of channels * bytes per sample
    return header.numChannels * (header.bitsPerSample / 8); // divide by 8 for bytesPerSample
}

/*
Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!

check50 cs50/problems/2023/x/reverse

Results for cs50/problems/2023/x/reverse generated by check50 v3.3.8
:) reverse.c exists
:) reverse.c compiles
:) reverse.c handles lack of input file
:) reverse.c creates an output file
:) reverse.c writes WAV header to output file
:) reverse.c reverses ascending scale
---
Execute the below to evaluate the style of your code using style50.

style50 reverse.c
*/