#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);
int convertIter(string input);
int convertSoln(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    //printf("%i\n", convertIter(input));

    // Convert string to int
    //printf("%i\n", convert(input));

    printf("%i\n", convertSoln(input));
    return 0;
}

int convertSoln(string input)
{
    // TODO
    int n = strlen(input);

    if (n == 0)
    {
        return 0;
    }
    char last_digit = input[n - 1];
    int converted_last_digit = last_digit - '0';
    input[n-1] = '\0';

    return converted_last_digit + 10 * convertSoln(input);
}
/*
Tips for building recursion.
1. Base case and what it could return.
2. Do the operation for the very first thing.
3. Break the problem into a smaller similar chunk and start the recursion.
4. When the stack pops, it should be returning the solution to the smaller problems

123

1*  last = 3, input = 123. return 3 + 10 * convert("12")
2*      last = 2, input = 12. return 2 + 10 * convert("1")
3*          last 1, input = "1". return 1 + 10 * convert("")

3*          return 1 + 10 * (0)
2*      return 2 + (10 * (1))
3*  return 3 + 10 * (2 + (10 * (1)))

= 123
*/

int convertRecur(string input, int numPlace)
{
    int strLen = strlen(input);
    int res = 0;
    char lastChar;

    if (strLen == 1)
    {
        return (input[0] - '0') * pow(10, numPlace);
    }
    else
    {
        lastChar = input[strLen - 1];
        input[strLen - 1] = '\0';
        // printf("char %c\n", lastChar);

        return (pow(10, numPlace) * (lastChar - '0')) + convertRecur(input, numPlace + 1);
    }
}
/*
123
(100 * 1) + (10 * 2) + (1 * 3)

first
lastChar = 3
input = "12"
numPlace = 0

call stack
1*  con("12", 1)
2*      con("1", 2)

Pop
2*      1 * 10^2 = 100
1*  10^1 * 2 + (100 ^^) = 120

final:
3 + 120 = 123
*/

int convert(string input)
{
    // TODO
    return convertRecur(input, 0);
}

int convertIter(string input)
{
    int res = 0;
    int len = strlen(input);
    for (int i = len - 1; i >= 0; i--)
    {
        // printf("-------------\n");
        // printf("%i\n", input[i] - '0');
        // printf("tens: %i\n", len - i - 1);
        // '0' = 48. e.g. "1" - '0' = 49 - 48 = decimal value 1
        res = res + (input[i] - '0') * (pow(10,(len - i - 1)));

        // printf("%i\n", res);
        // printf("-------------\n");
    }
    return res;
}