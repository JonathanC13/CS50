#include <cs50.h>
#include <stdio.h> // Usually don't import entire lib.

// function signatures.
int getStartSize();
int getEndSize(int iStartSize);
int calcYearsToReachEndPop(int iStartSize, int iEndSize);
void printYears(int iYears);

int main(void)
{
    // TODO: Prompt for start size
    int iStartSize = getStartSize();

    // TODO: Prompt for end size
    int iEndSize = getEndSize(iStartSize);

    // TODO: Calculate number of years until we reach threshold
    int iYears = calcYearsToReachEndPop(iStartSize, iEndSize);

    // TODO: Print number of years
    printYears(iYears);

}

/*
Prompt for start size
Param: N/a
Return: int for the starting population of llamas.
*/
int getStartSize()
{
    // Starting size must be greater than 9 or the population will become stagnet.
    int n = 0;
    do
    {
        n = get_int("Start size: ");
    }
    while(n < 9);

    return n;
}

/*
Prompt for end size
Param:
    int iStartSize: the starting population.
Return: int for the ending population of llamas.
*/
int getEndSize(int iStartSize)
{
    // Ending size must be >= to the starting size since the objective is to determine how many years it will take to reach the desired ending population.
    int n = 0;
    do
    {
        n = get_int("End size: ");
    }
    while(n < iStartSize);

    return n;
}

/*
Calculate number of years until we reach threshold
Param:
    int iStartSize: the starting population.
    int iEndSize: the end population
Return: int for the number of years it takes to reach/surpass the end population.
*/
int calcYearsToReachEndPop(int iStartSize, int iEndSize)
{
    int iCurrSize = iStartSize;
    // newCurrSize = currSize + (currSize/3) - (currSize/4)
    int iYears = 0;
    while(1)
    {
        // Exit condition. >999 for safety catch.
        if (iCurrSize >= iEndSize || iYears > 999){
            break;
        }

        iCurrSize = iCurrSize + (iCurrSize/3) - (iCurrSize/4);

        iYears ++;
    }

    return iYears;
}

/*
Print number of years
Param:
    int iYears: the number of years to print.
Return: void
*/
void printYears(int iYears)
{
    printf("Years: %i\n", iYears);
}


/*
Some test cases:
function: getStartSize
    Ranges; (-inf, 9), [9, +inf)
    Chosen input values:
        8   // exp result: reprompt
        9   // exp result: accepted

function: getEndSize(int iStartSize)
    Ranges; (-inf, iStartSize), [iStartSize, +inf)
    Chosen inputs:
        getStartSize: 9, getEndSize: 8  // exp result: reprompt
        getStartSize: 9, getEndSize: 9  // exp result: accepted

function: calcYearsToReachEndPop(int iStartSize, int iEndSize)
    Return ranges; [0], (0, 10], (10, 999)
    Chosen inputs:
        getStartSize: 9, getEndSize: 9  // exp result: 0
        getStartSize: 10, getEndSize: 18  // exp result: (0, 10]
        getStartSize: 11, getEndSize: 200  // exp result: (10, 999)


CS50 tests:
$ ./population
Start size: 1200
End size: 1300
Years: 1
$ ./population
Start size: -5
Start size: 3
Start size: 9
End size: 5
End size: 18
Years: 8
$ ./population
Start size: 20
End size: 1
End size: 10
End size: 100
Years: 20
$ ./population
Start size: 100
End size: 1000000
Years: 115


Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!
check50 cs50/labs/2023/x/population

Execute the below to evaluate the style of your code using style50.
style50 population.c
*/