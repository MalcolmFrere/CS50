/*
    CS50 - set4 - helpers.c
    Author: MalcolmFrere
    Date: 21th August 2022
    Version: 1.0
    Description : Apply a filter (grayscale, reflect, blur, edge) on BMP file
    Usage : function call by filter.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "helpers.h"

// Define index in BYTE for each color as file is BGR
#define RED 2
#define GREEN 1
#define BLUE 0

BYTE blur_pixel(int height, int width, int hPos, int wPos, BYTE *pixel, int color);
BYTE sobel_filter(int height, int width, int hPos, int wPos, BYTE *pixel, int color);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Parse image line by line
    for (int i = 0; i < height; i++)
    {
        // Parse image pixel by pixel
        for (int j = 0; j < width; j++)
        {
            // Define average of pixel luminosity value
            BYTE average = (BYTE)round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Assign all color channel with average value
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Parse image line by line
    for (int i = 0; i < height; i++)
    {
        // Parse image pixel by pixel
        for (int j = 0; j < (int)(width / 2); j++)
        {
            // Invert pixel by pixel on y axes
            RGBTRIPLE rgbTemp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = rgbTemp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Realize a copy of image in BYTE pointer to allow to address all colors using an index
    BYTE *pixels = malloc(height * width * 3 * sizeof(BYTE));
    if (pixels == NULL)
    {
        printf("Allocation memory error.\n");
        exit(-1);
    }
    memcpy(pixels, image, sizeof(RGBTRIPLE) * height * width);

    // Parse image line by line
    for (int i = 0; i < height; i++)
    {
        // Parse image pixel by pixel anc calculate blured pixel value for each color channel
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blur_pixel(height, width, i, j, pixels, RED);
            image[i][j].rgbtGreen = blur_pixel(height, width, i, j, pixels, GREEN);
            image[i][j].rgbtBlue = blur_pixel(height, width, i, j, pixels, BLUE);
        }
    }

    free(pixels);

    return;
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Realize a copy of image in BYTE pointer to allow to address all colors using an index
    BYTE *pixels = malloc(height * width * 3 * sizeof(BYTE));
    if (pixels == NULL)
    {
        printf("Allocation memory error.\n");
        exit(-1);
    }

    memcpy(pixels, image, sizeof(RGBTRIPLE) * height * width);


    // Parse image line by line
    for (int i = 0; i < height; i++)
    {
        // Parse image pixel by pixel anc calculate Sobel filter pixel value for each color channel
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = sobel_filter(height, width, i, j, pixels, RED);
            image[i][j].rgbtGreen = sobel_filter(height, width, i, j, pixels, GREEN);
            image[i][j].rgbtBlue = sobel_filter(height, width, i, j, pixels, BLUE);
        }
    }

    free(pixels);

    return;
}

// calculate average value of pixel in 3x3 matrice around is position for blur function
BYTE blur_pixel(int height, int width, int hPos, int wPos, BYTE *pixel, int color)
{
    float nb_pixels = 0, sum = 0;

    // On a square of 3x3 around pixel
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if (((hPos + i) >= 0) && ((hPos + i) < height) && ((wPos + j) >= 0) && ((wPos + j) < width))
            {
                // Sum the 9 pixels to realize average calculation
                sum += *(pixel + ((hPos + i) * width * 3) + ((wPos + j) * 3) + color);
                nb_pixels++;
            }
            else
            {
                // Don't do anything if we are behind the edge
            }
        }
    }

    // Return average
    return (BYTE)round(sum / nb_pixels);
}


// Apply Sobel filter on pixel
BYTE sobel_filter(int height, int width, int hPos, int wPos, BYTE *pixel, int color)
{
    int sumGx = 0, sumGy = 0;
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // On a square of 3x3 around pixel
    for (int i = -1; i < 2; i++)
    {
        // For each pixel in 3x3 array, apply Sobel filter
        for (int j = -1; j < 2; j++)
        {
            if (((hPos + i) >= 0) && ((hPos + i) < height) && ((wPos + j) >= 0) && ((wPos + j) < width))
            {
                sumGx += *(pixel + ((hPos + i) * width * 3) + ((wPos + j) * 3) + color) * Gx[i + 1][j + 1];
                sumGy += *(pixel + ((hPos + i) * width * 3) + ((wPos + j) * 3) + color) * Gy[i + 1][j + 1];
            }
            else
            {
                // Pixel is solid black if we are on the edge (0x00)
            }
        }
    }

    // return SQRT (sumGx^2 + sumGy^2) with a max of 255 and rounded to be a BYTE
    return (BYTE)round(fmin(sqrt((sumGx * sumGx) + (sumGy * sumGy)), 255));
}