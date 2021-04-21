#include "helpers.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    /*
        Calculates average rgb and sets every
        color with the same average value
    */
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            int average = round((red + green + blue) / 3.0);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    /*
        Calculates sepia with the provided formula, checks if
        the the result number is greater than 255 and applies
        to the image
    */
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            float sepiaRed = (.393 * originalRed) + (.769 * originalGreen) + (.189 * originalBlue);
            float sepiaGreen = (.349 * originalRed) + (.686 * originalGreen) + (.168 * originalBlue);
            float sepiaBlue = (.272 * originalRed) + (.534 * originalGreen) + (.131 * originalBlue);

            if (sepiaRed > 255.0)
            {
                sepiaRed = 255.0;
            }
            if (sepiaGreen > 255.0)
            {
                sepiaGreen = 255.0;
            }
            if (sepiaBlue > 255.0)
            {
                sepiaBlue = 255.0;
            }

            image[i][j].rgbtRed = round(sepiaRed);
            image[i][j].rgbtGreen = round(sepiaGreen);
            image[i][j].rgbtBlue = round(sepiaBlue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    /*
        Trades the first for the last pixel.
        i is the row.
        j is the first pixel (left) increasing to right.
        l is the last pixel (right) decreasing to left.
    */
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, l = width - 1; j < l; j++, l--)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][l];
            image[i][l] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    /*
        Creates copy of the original image
    */
    RGBTRIPLE copyImg[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copyImg[i][j] = image[i][j];
        }
    }

    /*
        Checks the position of the pixel, i is the row
        and j is the column of the pixel.
        Always select the pixel itself.
        Add the neighbour pixel if it exists.
    */

    for (int i = 0, red, green, blue, counter; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red = green = blue = counter = 0;

            red += copyImg[i][j].rgbtRed;
            green += copyImg[i][j].rgbtGreen;
            blue += copyImg[i][j].rgbtBlue;
            counter++;

            if (j - 1 >= 0) // center
            {
                red += copyImg[i][j - 1].rgbtRed;
                green += copyImg[i][j - 1].rgbtGreen;
                blue += copyImg[i][j - 1].rgbtBlue;
                counter++;
            }

            if (j + 1 < width) // right
            {
                red += copyImg[i][j + 1].rgbtRed;
                green += copyImg[i][j + 1].rgbtGreen;
                blue += copyImg[i][j + 1].rgbtBlue;
                counter++;
            }

            if (i - 1 >= 0) // top
            {
                red += copyImg[i - 1][j].rgbtRed;
                green += copyImg[i - 1][j].rgbtGreen;
                blue += copyImg[i - 1][j].rgbtBlue;
                counter++;
            }

            if (i - 1 >= 0 && j - 1 >= 0) // top left
            {
                red += copyImg[i - 1][j - 1].rgbtRed;
                green += copyImg[i - 1][j - 1].rgbtGreen;
                blue += copyImg[i - 1][j - 1].rgbtBlue;
                counter++;
            }

            if (i - 1 >= 0 && j + 1 < width) // top right
            {
                red += copyImg[i - 1][j + 1].rgbtRed;
                green += copyImg[i - 1][j + 1].rgbtGreen;
                blue += copyImg[i - 1][j + 1].rgbtBlue;
                counter++;
            }

            if (i + 1 < height) // bottom
            {
                red += copyImg[i + 1][j].rgbtRed;
                green += copyImg[i + 1][j].rgbtGreen;
                blue += copyImg[i + 1][j].rgbtBlue;
                counter++;
            }

            if (i + 1 < height && j - 1 >= 0) // bottom left
            {
                red += copyImg[i + 1][j - 1].rgbtRed;
                green += copyImg[i + 1][j - 1].rgbtGreen;
                blue += copyImg[i + 1][j - 1].rgbtBlue;
                counter++;
            }

            if (i + 1 < height && j + 1 < width) // bottom right
            {
                red += copyImg[i + 1][j + 1].rgbtRed;
                green += copyImg[i + 1][j + 1].rgbtGreen;
                blue += copyImg[i + 1][j + 1].rgbtBlue;
                counter++;
            }

            image[i][j].rgbtRed = round(red / (float) counter);
            image[i][j].rgbtGreen = round(green / (float) counter);
            image[i][j].rgbtBlue = round(blue / (float) counter);
        }
    }

    return;
}