#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
$ ./substitution YTNSHKVEFXRBAUQZCLWDMIPGJO
plaintext:  HELLO
ciphertext: EHBBQ

$ ./substitution VCHPRZGJNTLSKFBDQWAXEUYMOI
plaintext:  hello, world
ciphertext: jrssb, ybwsp

$ ./substitution yukfrnlbavmwzteogxhcipjsqd
plaintext:  This is CS50
ciphertext: Cbah ah KH50
*/

void printCipherText(string sKey, string sPlainText);

int main(int argc, string argv[])
{
    // Only accepts 1 command-line arguments.
    if (argc != 2)
    {
        printf("Please provide a cipher key in the command-line argument.\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26) // Only accepts 26 length.
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    int iLen = strlen(argv[1]);

    char cCurrLetter;
    for (int i = 0; i < iLen; i++)
    {
        cCurrLetter = argv[1][i];

        if (isalpha(cCurrLetter) == 0)
        {
            // Only accepts alphabetical characters.
            printf("Key must only contain alphabetical characters.\n");
            return 1;
        }

        for (int j = 0; j < iLen; j++)
        {
            if (cCurrLetter == argv[1][j] && i != j)
            {
                // No duplicate characters in the key.
                printf("Key must not contain duplicate letters.\n");
                return 1;
            }
        }
    }

    string sPlainText = get_string("plaintext:  ");
    printf("ciphertext: ");

    printCipherText(argv[1], sPlainText);

    printf("\n");
    return 0;
}

void printCipherText(string sKey, string sPlainText)
{
    string sRegAlpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char sReplace;
    for (int i = 0, iLen = strlen(sPlainText); i < iLen; i++)
    {
        sReplace = '\0';
        for (int j = 0, iLenReg = strlen(sRegAlpha); j < iLenReg; j++)
        {
            if (toupper(sPlainText[i]) == toupper(sRegAlpha[j]))
            {
                // found index, map to key's index
                sReplace = sKey[j];
                if (isupper(sPlainText[i]))
                {
                    sReplace = toupper(sReplace);
                }
                else
                {
                    sReplace = tolower(sReplace);
                }
                // printf("%c\n", sKey[j]);
                sPlainText[i] = sReplace;
                break;
            }
        }
    }
    printf("%s", sPlainText);
}

/*
Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!

check50 cs50/problems/2023/x/substitution

Results for cs50/problems/2023/x/substitution generated by check50 v3.3.8
:) substitution.c exists
:) substitution.c compiles
:) encrypts "A" as "Z" using ZYXWVUTSRQPONMLKJIHGFEDCBA as key
:) encrypts "a" as "z" using ZYXWVUTSRQPONMLKJIHGFEDCBA as key
:) encrypts "ABC" as "NJQ" using NJQSUYBRXMOPFTHZVAWCGILKED as key
:) encrypts "XyZ" as "KeD" using NJQSUYBRXMOPFTHZVAWCGILKED as key
:) encrypts "This is CS50" as "Cbah ah KH50" using YUKFRNLBAVMWZTEOGXHCIPJSQD as key
:) encrypts "This is CS50" as "Cbah ah KH50" using yukfrnlbavmwzteogxhcipjsqd as key
:) encrypts "This is CS50" as "Cbah ah KH50" using YUKFRNLBAVMWZteogxhcipjsqd as key
:) encrypts all alphabetic characters using DWUSXNPQKEGCZFJBTLYROHIAVM as key
:) does not encrypt non-alphabetical characters using DWUSXNPQKEGCZFJBTLYROHIAVM as key
:) handles lack of key
:) handles too many arguments
:) handles invalid key length
:) handles invalid characters in key
:) handles duplicate characters in uppercase key
:) handles duplicate characters in lowercase key
:) handles multiple duplicate characters in key
---
Execute the below to evaluate the style of your code using style50.

style50 substitution.c
*/