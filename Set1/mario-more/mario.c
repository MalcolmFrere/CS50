/*
    CS50 - set1 - mario.c
    Author: MalcolmFrere
    Date: 1st June 2022
    Version: 1.0
    Description : Print pyramide patern of specif height enter by user
*/

#include <cs50.h>
#include <stdio.h>

void print_patern(int line);
void print_gap(int gapSize);

int main(void)
{
    // declare height string and request value to user
    int height = 0;

    do
    {
        height = get_int("Height : ");
    }
    while (height <= 0 || height > 8);

    for (int i = 1; i <= height; i++)
    {
        print_gap(height - i);
        print_patern(i);
        print_gap(2);
        print_patern(i);

        // end of line
        printf("\n");
    }
}

// Function to display a specific quantity of '#' in function of parameter pass in input
void print_patern(int line)
{
    for (int patern = 1; patern <= line; patern++)
    {
        printf("#");
    }
}

// Function to display a specific quantity of space in function of parameter pass in input
void print_gap(int gapSize)
{
    for (int gap = 0; gap < gapSize; gap++)
    {
        printf(" ");
    }
}