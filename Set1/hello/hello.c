/*
    CS50 - set1 - hello.c
    Author: MalcolmFrere
    Date: 1st June 2022
    Version: 1.0
    Description : Print hello with user name entered by user
*/

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Declare name variable as string and require user name
    string name = get_string("What's your name? ");

    // Display "Hello, " with username, and new line caracter
    printf("Hello, %s\n", name);
}