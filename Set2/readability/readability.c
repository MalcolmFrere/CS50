/*
    CS50 - set2 - readability.c
    Author: MalcolmFrere
    Date: 3rd June 2022
    Version: 1.0
    Description : Define grade level readability based on an abstract of text
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int coleman_liau(int l, int w, int s);

int main(void)
{
    // Request test abstract to user
    string text = get_string("Text: ");

    // Declare variable to count letters (l), words (w) and sentences (s)
    int l = count_letters(text);
    int w = count_words(text);
    int s = count_sentences(text);

    // Calculate Coleman-Liau index
    int grade = coleman_liau(l, w, s);

    // We test result to display correct grade
    if (grade <= 0)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 0  && grade < 16)
    {
        printf("Grade %i\n", (int) round(grade));
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string text)
{
    int l = 0;
    char c;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Transform to lower case to simplify if statement
        c = tolower(text[i]);

        // If char is a letter, count it!
        if (c >= 'a' && c <= 'z')
        {
            l++;
        }
    }

    return l;
}

int count_words(string text)
{
    int w = 0;
    char c, prevC = '.' ;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Save previous chat to do not record two times word with punctuation sign + space
        prevC = c;
        c = text[i];

        // If char is a space and does not appears after a end of sentences
        if (c == ' ' && prevC != '.' && prevC != '!' && prevC != '?')
        {
            w++;
        }
        else if (c == '.' || c == '!' || c == '?')
        {
            w++;
        }
    }

    return w;
}

int count_sentences(string text)
{
    int s = 0;
    char c;

    // For each character equal to period, exclamation or question mark, increment sentence counter.
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        c = text[i];

        if (c == '.' || c == '!' || c == '?')
        {
            s++;
        }
    }


    return s;
}

int coleman_liau(int l, int w, int s)
{
    // Calculate average number of letters and sentences for hundred words
    float lphw = (float) l / (float) w * 100.;
    float sphw = (float) s / (float) w * 100.;

    float grade = (0.0588 * lphw) - (0.296 * sphw) - 15.8;

    return (int) round(grade);
}