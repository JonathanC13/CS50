#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
//              A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q   R  S  T  U  V  W  X  Y  Z
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int iLen = strlen(word);
    char cCurrentChar;
    int iTotalScore = 0;
    int iCharA = 'A';
    for (int i = 0; i < iLen; i++)
    {
        cCurrentChar = toupper(word[i]);
        if (isalpha(cCurrentChar))
        {
            // subtract 65 from the decimal value of the char to get the index for the character score in array POINTS[]
            iTotalScore += POINTS[cCurrentChar - iCharA];
        }
    }
    return iTotalScore;
}

/*
Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!

check50 cs50/labs/2023/x/scrabble

Results for cs50/labs/2023/x/scrabble generated by check50 v3.3.8
:) scrabble.c exists
:) scrabble.c compiles
:) handles letter cases correctly
:) handles punctuation correctly
:) correctly identifies 'Question?' and 'Question!' as a tie
:) correctly identifies 'drawing' and 'illustration' as a tie
:) correctly identifies 'hai!' as winner over 'Oh,'
:) correctly identifies 'COMPUTER' as winner over 'science'
:) correctly identifies 'Scrabble' as winner over 'wiNNeR'
:) correctly identifies 'pig' as winner over 'dog'
:) correctly identifies 'Skating!' as winner over 'figure?'

---

Execute the below to evaluate the style of your code using style50.

style50 scrabble.c
*/