/*
    CS50 - set2 - substitution.c
    Author: MalcolmFrere
    Date: 8th June 2022
    Version: 1.0
    Description : Realize a "cipher" encryption
    Usage : substitution <key> with <key> assigning a letter for each place in alphabet
*/

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string convert_tolower(string key);
string cipher_crypt(string plaintext, string key);
bool check_double(string key);

// Parameters
#define ALPHABET_SIZE 26


// Error codes of application
#define ERROR_QUANTITY 1   // 10
#define ERROR_SIZE 1       // 20
#define ERROR_CHAR 1       // 30
#define ERROR_DOUBLE 1     // 40

int main(int argc, string argv[])
{
    string key = " ";

    // Check if parameters are correct (1 parameter equal to 26 chars)
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        // ERROR Code 10 : Bad quantity of parameter
        return ERROR_QUANTITY;
    }
    else if (strlen(argv[1]) != ALPHABET_SIZE)
    {
        printf("Key must contain 26 characters.\n");
        // ERROR Code 20 : Key size is incorrect
        return ERROR_SIZE;
    }
    else
    {
        key = convert_tolower(argv[1]);

        // Check if char is missing after convertion to lower which will supress non alphabetic char
        if (strlen(key) != ALPHABET_SIZE)
        {
            printf("Key must contain letter characters only.\n");
            // ERROR Code 30 : Key contains unauthorized char
            return ERROR_CHAR;
        }

        // Check repeated characters
        if (check_double(key))
        {
            printf("Key contains 2 times same letter.\n");
            return ERROR_DOUBLE;
        }
    }

    // Request text to encrypt to user
    string plaintext = get_string("plaintext:  ");

    string ciphertext = cipher_crypt(plaintext, key);

    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

// Function which transform key in lower case and return it. If char is not a letter, refuse char and return key with a char less.
string convert_tolower(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (tolower(key[i]) >= 'a' && tolower(key[i]) <= 'z')
        {
            key[i] = tolower(key[i]);
        }
    }

    return key;
}

// Funcion applying cipher encryption by replacing char in the plaintext by char in the key
string cipher_crypt(string plaintext, string key)
{
    string ciphertext = plaintext;

    // For each char search equivalent char in key depending if it is a lower or an upper case.
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (tolower(plaintext[i]) >= 'a' && tolower(plaintext[i]) <= 'z')
        {
            if (isupper(plaintext[i]))
            {
                ciphertext[i] = toupper(key[(int) plaintext[i] - 'A']);
            }
            else
            {
                ciphertext[i] = key[(int) plaintext[i] - 'a'];
            }
        }
        else
        {
            // Do nothing because current char is not a letter
        }
    }

    return ciphertext;
}

// Return true if a character present is missing in key
bool check_double(string key)
{
    bool alphabet[ALPHABET_SIZE] = {false};
    bool check = true;

    for (int i = 0, n = strlen(key); i < n; i++)
    {
        alphabet[key[i] - 'a'] = true;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        check = check && alphabet[i];
    }

    return !check;
}