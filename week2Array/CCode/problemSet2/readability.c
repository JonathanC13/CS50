#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/*
One such readability test is the Coleman-Liau index.
The Coleman-Liau index of a text is designed to output that (U.S.) grade level that is needed to understand some text.
The formula is:

index = 0.0588 * L - 0.296 * S - 15.8

where L is the average number of letters per 100 words in the text,
    and S is the average number of sentences per 100 words in the text.
*/

int count_letters(string text, int iLen);
int count_words(string text, int iLen);
int count_sentences(string text, int iLen);

int main(void)
{
    string sText = get_string("Text: ");
    int iLen = strlen(sText);

    int iNumLetters = count_letters(sText, iLen);
    // printf("Letters: %i\n", iNumLetters);

    int iNumWords = count_words(sText, iLen);
    // printf("Words: %i\n", iNumWords);

    int iNumSent = count_sentences(sText, iLen);
    // printf("Sentences: %i\n", iNumSent);

    /*
    index = 0.0588 * L - 0.296 * S - 15.8

    where L is the average number of letters per 100 words in the text,
    and S is the average number of sentences per 100 words in the text.
    */
    int index = round((0.0588 * ((float) iNumLetters / (float) iNumWords) * 100) -
                      (0.296 * ((float) iNumSent / (float) iNumWords) * 100) - 15.8);
    // printf("index: %f\n", index);

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
    return 0;
}

int count_letters(string text, int iLen)
{
    int iNumLetters = 0;

    for (int i = 0; i < iLen; i++)
    {
        if (isalpha(text[i]))
        {
            iNumLetters++;
        }
    }

    return iNumLetters;
}

/*
You may assume that a sentence:

will contain at least one word;
will not start or end with a space; and
will not have multiple spaces in a row.
*/
int count_words(string text, int iLen)
{
    int iNumWords = 0;
    int iAlphaDetectedFlag = 1;
    int iSpaceFlag = 1;
    char cCurr;

    for (int i = 0; i < iLen; i++)
    {
        cCurr = text[i];
        if (isalpha(cCurr))
        {
            iAlphaDetectedFlag = 0;
            iSpaceFlag = 1;
        }
        else if (isspace(cCurr) && iAlphaDetectedFlag == 0)
        {
            // if space detected and previously there was alpha, increment iNumWords
            iNumWords++;
            iAlphaDetectedFlag = 1;
            iSpaceFlag = 0;
        }
        // else continue char by char
    }

    if (iAlphaDetectedFlag == 0)
    {
        iNumWords += 1; // Due if the end of the string and still a word.
    }

    return iNumWords;
}

// You should consider any sequence of characters that ends with a . or a ! or a ? to be a sentence
int count_sentences(string text, int iLen)
{
    int iNumSentences = 0;

    for (int i = 0; i < iLen; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            iNumSentences++;
        }
    }

    return iNumSentences;
}

/*
Execute the below to evaluate the correctness of your code further using check50.
But be sure to compile and test it yourself as well!

check50 cs50/problems/2023/x/readability

Results for cs50/problems/2023/x/readability generated by check50 v3.3.8
:) readability.c exists
:) readability.c compiles
:) handles single sentence with multiple words
:) handles punctuation within a single sentence
:) handles more complex single sentence
:) handles multiple sentences
:) handles multiple more complex sentences
:) handles longer passages
:) handles questions in passage
:) handles reading level before Grade 1
:) handles reading level at Grade 16+
-----

Execute the below to evaluate the style of your code using style50.

style50 readability.c
*/


