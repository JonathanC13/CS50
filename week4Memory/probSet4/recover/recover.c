#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

#define FATSIZE (512)
#define JPEGHEADERSIZE 4
#define FILENAMESIZE 3
#define JPEGEXTSIZE 5
// 0xff 0xd8 0xff 0xeXX
// 0xeXX mask with AND 0x1110 0000 to see if valid header

void padNumericalName(int value, char *retName, int size)
{
    int lastIndex = size - 1;

    for (int i = lastIndex; i >= 0; i--)
    {
        if (value == 0)
        {
            retName[i] = '0';
        }
        else
        {
            retName[i] = (value % 10) + 48;
            value = floor(value / 10);
        }
    }
}

void addJPEGExt(char *retName, int len)
{
    int jpegExtLen = JPEGEXTSIZE + 1;
    char jpegExt[JPEGEXTSIZE + 1] = {'.', 'j', 'p', 'e', 'g', '\0'};

    for (int i = 0; i <= jpegExtLen; i++)
    {
        retName[len - i] = jpegExt[jpegExtLen - i];
    }
}

void createFileName(int value, char *retName, int lenOfFileNameWext, int lenOfFileName)
{
    padNumericalName(value, retName, lenOfFileName);
    addJPEGExt(retName, lenOfFileNameWext);
}

// ./recover card.raw
int main(int argc, char *argv[])
{
    // check number of command-line arguments.
    if (argc != 2)
    {
        printf("Usage: recover [forensic image]\n");
        return 1;
    }

    // attempt to open input file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    int fileCount = 0;

    int fileNameLen = FILENAMESIZE + JPEGEXTSIZE + 1; // + 1 for the '\0'
    char sFileName[fileNameLen];

    // createFileName(fileCount, sFileName, fileNameLen, FILENAMESIZE); // did it the hard way.
    // sprintf(sFileName, "%03i.jpeg", fileCount); // use this
    /*
    for (int i = 0; i < fileNameLen; i++)
    {
        printf("%c\n", sFileName[i]);
    }
    */

    // printf("%s\n", sFileName);

    // attempt to open initial output file.
    /*
    FILE *output = fopen(sFileName, "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    */

    FILE *output = NULL;

    BYTE *fatBlock = malloc(FATSIZE * sizeof(BYTE));
    if (fatBlock == NULL)
    {
        printf("fatBlock, no allocated memory.\n");
        return 2;
    }

    // Read 1 FAT, 512 B, at a time.
    // If the FAT block starts with the header of JPEG start a new file.
    while (fread(fatBlock, FATSIZE, 1, input) == 1)
    {
        // check if JPEG header is at the start.
        if (fatBlock[0] == 0xff && fatBlock[1] == 0xd8 && fatBlock[2] == 0xff && ((fatBlock[3] & 0xe0) == 0xe0))
        {
            // FAT block starts with a JPEG header, close current file and start new one and store current FAT block into it.
            if (fileCount > 0)
            {
                fclose(output);
            }

            // padNumericalName(fileCount, sFileName, FILENAMESIZE);
            snprintf(sFileName, fileNameLen, "%03i.jpg", fileCount); // use this

            output = fopen(sFileName, "w");
            if (output == NULL)
            {
                free(fatBlock);
                printf("Could not open file.\n");
                return 1;
            }

            fileCount++;
        }

        if (output != NULL)
        {
            if (fwrite(fatBlock, FATSIZE, 1, output) < 1 && output != NULL)
            {
                break;
            }
        }
    }

    free(fatBlock);

    fclose(input);
    fclose(output);

    return 0;
}
// ./recover card.raw

/*
Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!

check50 cs50/problems/2023/x/recover

Results for cs50/problems/2023/x/recover generated by check50 v3.3.8
:) recover.c exists.
:) recover.c compiles.
:) handles lack of forensic image
:) recovers 000.jpg correctly
:) recovers middle images correctly
:) recovers 049.jpg correctly
:) program is free of memory errors
---
Execute the below to evaluate the style of your code using style50.

style50 recover.c
*/