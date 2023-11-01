#include <cs50.h>
#include <stdio.h>
#include <math.h>

int collatz(int i)
{
    if (i == 1)
    {
        return 0;
    }
    else if (i % 2 == 0)
    {
        return 1 + collatz(floor(i / 2));
    }
    else
    {
        return 1 + collatz((3 * i) + 1);
    }
}

int main(void)
{
    int i = get_int("Enter a number: ");
    printf("Steps %i\n", collatz(i));
}