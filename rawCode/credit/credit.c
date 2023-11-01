#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// https://cs50.harvard.edu/x/2023/psets/1/credit/

// Use long since the integer value of a credit card is much larger than what an `int` can hold.

#define AMEXLENGTH 15
#define MASTERLENGTH 16
#define VISALENGTH1 13
#define VISALENGTH2 16

long getCreditCardNumber();
bool validateCardType(long lInputNumber, int iInputNumberLen, int iCardTypeNumLen, int iNumOfStartDigits, int arrValidStartDigits[],
                      int iArrSize);
bool passLuhnAlgorithm(long lNum);
int validateNumber(long lNumber);
void inputTests();

int main(void)
{
    // tests
    // inputTests();
    // return 0;
    // Get the user's input for the credit card number
    long lNumber = getCreditCardNumber();

    // Determine if the number has the potential to be an [AMEX, MASTERCARD, VISA] by checking its leading numbers and length.
    // Then validate the checkSum with Luhn's algorithm.
    validateNumber(lNumber);

    return 0;
}

/*
Get the user's input for the credit card number. Must be a data type `long`. Assumption that there will be no inputs with leading
zeros.
Params: N/a
Return: int, the long value
*/
long getCreditCardNumber()
{
    long n;
    do
    {
        n = get_long("Number: ");
    }
    while (n < 0);

    return n;
}

/*
Validate the number by checking the length, the start digits, and then the check sum.
Params:
    long lNumber
Return: int, just to exit.
*/
int validateNumber(long lNumber)
{
    string sPotenCard = "";
    // get length of the number
    int iLen = floor(log10(lNumber)) + 1;

    int arrValidStartDigitsAMEX[] = {34, 37};
    int iArrSizeAMEX = sizeof(arrValidStartDigitsAMEX) / sizeof(arrValidStartDigitsAMEX[0]);

    int arrValidStartDigitsMASTER[] = {51, 52, 53, 54, 55};
    int iArrSizeMASTER = sizeof(arrValidStartDigitsMASTER) / sizeof(arrValidStartDigitsMASTER[0]);

    int arrValidStartDigitsVISA[] = {4};
    int iArrSizeVISA = sizeof(arrValidStartDigitsVISA) / sizeof(arrValidStartDigitsVISA[0]);

    // check if a valid length and starting digits.
    if (validateCardType(lNumber, iLen, AMEXLENGTH, 2, arrValidStartDigitsAMEX, iArrSizeAMEX))
    {
        sPotenCard = "AMEX";
    }
    else if (validateCardType(lNumber, iLen, MASTERLENGTH, 2, arrValidStartDigitsMASTER, iArrSizeMASTER))
    {
        sPotenCard = "MASTERCARD";
    }
    else if (validateCardType(lNumber, iLen, VISALENGTH1, 1, arrValidStartDigitsVISA, iArrSizeVISA) ||
             validateCardType(lNumber, iLen, VISALENGTH2, 1, arrValidStartDigitsVISA, iArrSizeVISA))
    {
        sPotenCard = "VISA";
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }

    // Luhn
    if (!passLuhnAlgorithm(lNumber))
    {
        sPotenCard = "INVALID";
    }

    printf("%s\n", sPotenCard);
    return 0;
}

/*
Check if the number is potentially an MASTERCARD card by validating its length and starting digits. Check sum is performed later.
Params:
    long lNumber: card number provided by the user.
    int iLen: length of the card number.
Return: bool, true for potentially it could be an AMEX and false for not.
*/
bool validateCardType(long lInputNumber, int iInputNumberLen, int iCardTypeNumLen, int iNumOfStartDigits, int arrValidStartDigits[],
                      int iArrSize)
{
    int iStartDigits = floor(lInputNumber / pow(10, iCardTypeNumLen - iNumOfStartDigits));

    if (iInputNumberLen == iCardTypeNumLen)
    {
        for (int i = 0; i < iArrSize; i++)
        {
            if (iStartDigits == arrValidStartDigits[i])
            {
                return true;
            }
        }
    }

    return false;
}

bool passLuhnAlgorithm(long lNum)
{
    // check sum, mod 10 to get the digit in the one's position.
    int iToggle = 0; // 0 means add and 1 means num*2 then add to the sum.

    long lNumRemaining = lNum;
    long lDigitExtracted = 0;
    long lSum = 0;

    do
    {
        lDigitExtracted = (lNumRemaining % 10);
        // printf("Extr: %ld\n", lDigitExtracted);
        // printf("Toggle: %i\n", iToggle);
        if (iToggle == 0)
        {
            lSum += lDigitExtracted;
            iToggle = 1;
        }
        else
        {
            lDigitExtracted *= 2;
            lSum += lDigitExtracted % 10;
            lSum += (int) floor(lDigitExtracted / 10) % 10;

            iToggle = 0;
        }
        // printf("Curr sum: %ld\n", lSum);
        lNumRemaining = floor(lNumRemaining / 10);
        // printf("Remaining: %ld\n\n", lNumRemaining);
    }
    while (lNumRemaining != 0);
    // printf("Sum: %ld\n", lSum);
    if (lSum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void inputTests()
{
    char sArrCardType[] = {'A', 'A', 'A', 'M', 'M', 'V', 'V', 'V', 'V', 'M', 'M', 'I'};
    long iArrNum[] = {378282246310005, 371449635398431,  378734493671000,  5555555555554444, 5105105105105100, 4111111111111111,
                      4222222222222,   4999991111111113, 4999992222222229, 5199999999999991, 5299999999999990, 5299999999999991};
    int iSize = sizeof(iArrNum) / sizeof(iArrNum[0]);

    for (int i = 0; i < iSize; i++)
    {
        printf("Type: %c\n", sArrCardType[i]);
        validateNumber(iArrNum[i]);
        printf("\n");
    }
}

/*
You can also execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself
as well!

check50 cs50/problems/2023/x/credit

Results for cs50/problems/2023/x/credit generated by check50 v3.3.8
:) credit.c exists
:) credit.c compiles
:) identifies 378282246310005 as AMEX
:) identifies 371449635398431 as AMEX
:) identifies 5555555555554444 as MASTERCARD
:) identifies 5105105105105100 as MASTERCARD
:) identifies 4111111111111111 as VISA
:) identifies 4012888888881881 as VISA
:) identifies 4222222222222 as VISA
:) identifies 1234567890 as INVALID
:) identifies 369421438430814 as INVALID
:) identifies 4062901840 as INVALID
:) identifies 5673598276138003 as INVALID
:) identifies 4111111111111113 as INVALID
:) identifies 4222222222223 as INVALID

----
Execute the below to evaluate the style of your code using style50.

style50 credit.c
*/