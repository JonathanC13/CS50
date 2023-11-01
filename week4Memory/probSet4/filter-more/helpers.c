#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
/*
To make a pixel to grayscale R, G, and B have to be the same value.
Take the average of the red, green, and blue values to determine what shade of grey to make the new pixel.
*/
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int res = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            res = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = res;
        }
    }
    return;
}

// Convert image to sepia, which gives images an old-timey feel by making the whole image look a bit reddish-brown.
/*
sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue
sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue

Of course, the result of each of these formulas may not be an integer, but each value could be rounded to the nearest integer.
It’s also possible that the result of the formula is a number greater than 255, the maximum value for an 8-bit color value.
In that case, the red, green, and blue values should be capped at 255. As a result,
we can guarantee that the resulting red, green, and blue values will be whole numbers between 0 and 255, inclusive.
*/
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int r, g, b = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // red
            r = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            if (r > 255)
            {
                r = 255;
            }

            // green
            g = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            if (g > 255)
            {
                g = 255;
            }


            // blue
            b = round((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue));
            if (b > 255)
            {
                b = 255;
            }

            image[i][j].rgbtRed = r;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtBlue = b;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, midCol = ceil(width / 2); j < midCol; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

RGBTRIPLE getBlurBoxAvg(int height, int width, RGBTRIPLE image[height][width], int currRow, int currCol)
{
    int r = 0;
    int g = 0;
    int b = 0;
    float count = 0.0;

    RGBTRIPLE ret;

    int boxRadius = 1;

    int topRow = currRow - boxRadius;
    int botRow = currRow + boxRadius;
    int leftCol = currCol - boxRadius;
    int rightCol = currCol + boxRadius;

    for (int i = topRow; i <= botRow; i++)
    {
        for (int j = leftCol; j <= rightCol; j++)
        {
            if (i < 0 || i >= height || j < 0 || j >= width)
            {
                continue;
            }

            r += image[i][j].rgbtRed;
            g += image[i][j].rgbtGreen;
            b += image[i][j].rgbtBlue;
            count++;
        }
    }

    ret.rgbtRed = round(r / count);
    ret.rgbtGreen = round(g / count);
    ret.rgbtBlue = round(b / count);

    return ret;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    // will be freed off of stack when this function returns so pointing the 'image' to 'copy' doesn't work.

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = getBlurBoxAvg(height, width, image, i, j);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}
/*
$ ./filter -g ./images/tower.bmp OUTFILE.bmp
$ ./filter -s ./images/yard.bmp OUTFILE.bmp
$ ./filter -r ./images/courtyard.bmp OUTFILE.bmp
$ ./filter -b ./images/stadium.bmp OUTFILE.bmp
*/

/*
If `val` is over the `max` return the `max` else `val`
*/
int capIfOverMax(int val, int max)
{
    if (val > max)
    {
        return max;
    }
    else
    {
        return val;
    }
}

RGBTRIPLE edgePixelRGB(int height, int width, RGBTRIPLE image[height][width], int currRow, int currCol)
{
    RGBTRIPLE temp;

    int boxRadius = 1;
    int topRow = currRow - 1;
    int botRow = currRow + 1;
    int leftCol = currCol - 1;
    int rightCol = currCol + 1;

    // Gx
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    // Gy
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    int rowOfGs = 0;
    int colOfGs = 0;

    int iGxSumR = 0;
    int iGxSumG = 0;
    int iGxSumB = 0;

    int iGySumR = 0;
    int iGySumG = 0;
    int iGySumB = 0;

    for (int i = topRow; i <= botRow; i++)
    {
        colOfGs = 0;
        for (int j = leftCol; j <= rightCol; j++)
        {
            if (i < 0 || i >= height || j < 0 || j >= width)
            {
                // If target pixel is on an edge of the image, the out of bounds are treated as (0, 0, 0). Therefore, no special
                // calculation, just skip as usual.
                colOfGs++; // just move colOfGs.
                continue;
            }

            iGxSumR += (image[i][j].rgbtRed * Gx[rowOfGs][colOfGs]);
            iGxSumG += (image[i][j].rgbtGreen * Gx[rowOfGs][colOfGs]);
            iGxSumB += (image[i][j].rgbtBlue * Gx[rowOfGs][colOfGs]);

            iGySumR += (image[i][j].rgbtRed * Gy[rowOfGs][colOfGs]);
            iGySumG += (image[i][j].rgbtGreen * Gy[rowOfGs][colOfGs]);
            iGySumB += (image[i][j].rgbtBlue * Gy[rowOfGs][colOfGs]);

            colOfGs++;
        }
        rowOfGs++;
    }

    int iGFinalRed = capIfOverMax((int) round(sqrt(pow(iGxSumR, 2) + pow(iGySumR, 2))), 255);
    int iGFinalGreen = capIfOverMax((int) round(sqrt(pow(iGxSumG, 2) + pow(iGySumG, 2))), 255);
    int iGFinalBlue = capIfOverMax((int) round(sqrt(pow(iGxSumB, 2) + pow(iGySumB, 2))), 255);

    temp.rgbtRed = iGFinalRed;
    temp.rgbtGreen = iGFinalGreen;
    temp.rgbtBlue = iGFinalBlue;

    return temp;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // need a copy to store the new image pixel values, since we will use the original `image` to calculate and don't want to
    // use modified pixel values in our calculation of the new values.
    RGBTRIPLE copy[height][width];

    // need to evaluate each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = edgePixelRGB(height, width, image, i, j);
        }
    }

    // have to copy the new pixal values from `copy` to `image` addresses;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}
// $ ./filter -e ./images/stadium.bmp OUTFILE.bmp

// check50 cs50/problems/2023/x/filter/more -o json --output-file tests.json

/*
Be sure to test all of your filters on the sample bitmap files provided!

Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!

check50 cs50/problems/2023/x/filter/more

Results for cs50/problems/2023/x/filter/more generated by check50 v3.3.8
:) helpers.c exists
:) filter compiles
:) grayscale correctly filters single pixel with whole number average
:) grayscale correctly filters single pixel without whole number average
:) grayscale leaves alone pixels that are already gray
:) grayscale correctly filters simple 3x3 image
:) grayscale correctly filters more complex 3x3 image
:) grayscale correctly filters 4x4 image
:) reflect correctly filters 1x2 image
:) reflect correctly filters 1x3 image
:) reflect correctly filters image that is its own mirror image
:) reflect correctly filters 3x3 image
:) reflect correctly filters 4x4 image
:) blur correctly filters middle pixel
:) blur correctly filters pixel on edge
:) blur correctly filters pixel in corner
:) blur correctly filters 3x3 image
:) blur correctly filters 4x4 image
:) edges correctly filters middle pixel
:) edges correctly filters pixel on edge
:) edges correctly filters pixel in corner
:) edges correctly filters 3x3 image
:) edges correctly filters 4x4 image
---
Execute the below to evaluate the style of your code using style50.

style50 helpers.c
*/
