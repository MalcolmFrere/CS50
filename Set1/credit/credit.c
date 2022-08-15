/*
    CS50 - set1 - credit.c
    Author: MalcolmFrere
    Date: 1st June 2022
    Version: 1.1
    Description : Check validity of credit card number enter by user
*/

#include <cs50.h>
#include <math.h>
#include <stdio.h>

bool check_Luhn(long ccNumber);
int check_len(long ccNumber);
string check_brand(long ccNumber);
int digit(long ccNumber, int pos);

int main(void)
{
    long ccNumber = 0;
    bool valid = false;

    // Get credit card number
    ccNumber = get_long("Number : ");

    // Check size and validity of credit card number
    valid = check_Luhn(ccNumber) && check_len(ccNumber) >= 13 && check_len(ccNumber) <= 16;

    // If number is valid print credit card brand name if known else print 'INVALID'
    if (valid)
    {
        printf("%s", check_brand(ccNumber));
    }
    else
    {
        printf("INVALID\n");
    }
}

// Calculate checksum of credit card number and return TRUE if it is correct
bool check_Luhn(long ccNumber)
{
    int evenSum = 0, oddSum = 0;

    // STEP 1: Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together
    for (int i = 2; i <= check_len(ccNumber); i = i + 2)
    {
        if (digit(ccNumber, i) * 2 > 9)
        {
            // if current digit times 2 is above 9 reuse digit function to add together digit of result and finaly add it to evenSum
            evenSum = evenSum + (digit(digit(ccNumber, i) * 2, 1) + digit(digit(ccNumber, i) * 2, 2));
        }
        else
        {
            evenSum = evenSum + digit(ccNumber, i) * 2;
        }
    }

    // STEP 2: Add the sum (evenSum) to the sum of the digits that weren’t multiplied by 2 (oddSum)
    for (int i = 1; i <= check_len(ccNumber); i = i + 2)
    {
        oddSum = oddSum + digit(ccNumber, i);
    }

    // STEP 3: If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
    return ((evenSum + oddSum) % 10 == 0);
}

// Calculate and return lenght of credit card number
int check_len(long ccNumber)
{
    long currNumber = ccNumber;
    int lenght = 0;

    // Parse credit card number digit by digit to know the size
    do
    {
        lenght++;

        // Cast return of pow in long instead of double. Round return of 'pow' to do not have floating point issue.
        currNumber = ccNumber % ((long) round(pow(10, lenght)));
    }
    while (currNumber != ccNumber);

    return lenght;
}

// Verify and return credit card brand
string check_brand(long ccNumber)
{
    string brand;
    int firstTwo = digit(ccNumber, check_len(ccNumber)) * 10 + digit(ccNumber, check_len(ccNumber) - 1);

    // AMEX begin by 34 or 37
    if (firstTwo == 34 || firstTwo == 37)
    {
        return "AMEX\n";
    }
    // MASTERCARD begin by 51 or 52 or 53 or 54 or 55
    else if (firstTwo == 51 || firstTwo == 52 || firstTwo == 53 || firstTwo == 54 || firstTwo == 55)
    {
        return "MASTERCARD\n";
    }
    // VISA begin by 4
    else if (digit(ccNumber, check_len(ccNumber)) == 4)
    {
        return "VISA\n";
    }
    else
    {
        return "INVALID\n";
    }


}

// Return a truncature of ccNumber to the requested position : (ccNumber % (10^pos) / (10^pos / 10))
int digit(long ccNumber, int pos)
{
    return (ccNumber % ((long) round(pow(10, pos)))) / ((round(pow(10, pos))) / 10);
}