#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];
/*
e.g. 1 2 3
-   1   2   3
1       1   1
2           1
3

e.g. 2 3 1
-   1   2   3
1       1   1
2   1       2
3   1
*/

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];
/*
-   1   2   3
1       1   1
2
3

1 -> 2
1 -> 3
*/

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void printPref();
void printPairs();
void printLocked();

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        // printPref();

        printf("\n");
    }

    add_pairs();
    // printPairs();
    sort_pairs();
    // printPairs();
    lock_pairs();
    // printLocked();
    print_winner();
    return 0;
}

void printPref()
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }
}

void printPairs()
{
    for (int i = 0; i < pair_count; i++)
    {
        printf("Pair %i: W: %i, L: %i\n", i, pairs[i].winner, pairs[i].loser);
    }
    printf("\n");
}

void printLocked()
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i", locked[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        // check if name exists
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // iterate the rankings from the voter
    for (int i = 0; i < candidate_count - 1; i++)
    {
        // for each ranked candidate, iterate the remaining ranks to get the ones it is preferred over
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

/*
e.g. 1 2 3
-   1   2   3
1       1   1
2           1
3

e.g. 2 3 1
-   1   2   3
1       1   1
2   1       2
3   1

Tie scenario: 1 is preferred over 2 1 time, but also 2 is preferred over 1 1 time

what would be in pairs for this example:
pairs
1. W: 2, L:3
*/
// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    int strengthCur = 0;
    int strengthNext = 0;
    // Bubble sorting
    pair temp;
    int count = -1;
    while (count != 0)
    {
        count = 0;
        for (int i = 0; i < pair_count - 1; i++)
        {
            strengthCur = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
            strengthNext =
                preferences[pairs[i + 1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner];
            if (strengthCur < strengthNext)
            {
                temp = pairs[i + 1];
                pairs[i + 1] = pairs[i];
                pairs[i] = temp;

                count++;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
// bool locked[MAX][MAX];
/*
-   0   1   2
0
1   1
2

1 -> 0

for cycle
0 -> 2  // row above, flip then + 1 in col
2 -> 1  // row below, + 1 each, check the diagonal

OR

0 -> 2

for cycle
1 -> 0  // row below, + 1 each
2 -> 1  // row below, + 1 each

-   0   1   2
0           1
1   1
2       1


eg 2. no cycle
-   0   1   2
0
1   1       1
2   1

1 -> 0
2 -> 0
1 -> 2
*/
bool checkCycles(int winner, int loser)
{
    bool cycleFlag = false;
    // base case, if winner = loser for sure a cycle. Keep param winner consistent and changing the loser
    if (winner == loser)
    {
        return true;    // return true due to cycle exists
    }
    else
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[loser][i] == true)
            {
                cycleFlag = checkCycles(winner, i);
                // need to keep passing in the original winner so that you know if the cycle completes (base case)

            }

            if (cycleFlag == true)
            {
                break;
            }
        }
    }
    return cycleFlag;
}
/*
-   0   1   2   3
0       1
1               1
2
3   1*

say adding 3 -> 0
curr locked:
    0 -> 1
    1 -> 3

Call stack:
checkCycles(3, 0); sees locked[0][1] == true calls
    checkCycles(3, 1); sees locked[1][3] == true calls
        checkCycles(3, 3); base case
POP:
        true
    true
true


---
-   0   1   2   3
0       1*
1           1   1
2
3

Call stack:
1*  check(0, 1); sees locked[1][2] == true calls
2*      check(0, 2); no locked in row 2, and completes loop. DONE

POP:
2*      Returns false after loop. DONE
1*  Since 2* did not return true Continues loop

Call stack:
1*  check(0, 1); sees locked[1][3] == true
3*      check(0, 3); no locked in row 3, and so it completes the loop DONE

POP:
3*      Returns false after loop
4*  Since 3* didn't return true, continues loop. Finishes loop and finally returns false. (means base case condition never met)

pop

*/

void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        if (checkCycles(pairs[i].winner, pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}
/*
// TODO
    int cycleVal = pow(2, candidate_count) - 1;
    // if a binary result decimal value equals this, then it means adding into the locked
    // pair will create a cycle.
    // printf("cycle val: %i", cycleVal);
    int binResult;
    int binTemp;
    int checkRow = 0;
    int colVal = 0;

    for (int i = 0; i < pair_count; i++)
    {
        // build binary of all the rows including the one being that you are checking if it can be added.
        // if result is all 1s, eg. of 3 candidates and result is 111 (including the row you are looking to add.)
        // then do NOT, it will create a cycle.
        // NOTE, do not need to consider self cycle like 0 -> 1 and 1 -> 0 due to add_pair not adding it.
        checkRow = pairs[i].winner;

        binResult = 0b0; /// reset to 0
        binResult = ((binResult << pairs[i].loser) | 1) << (candidate_count - pairs[i].loser - 1);
        // since loser is the col, shift by the cols,
        // OR 1 to set it to 1, then shift in the
        // trailing 0s.

        // printLocked();
        // printf("Pair %i: W: %i, L: %i\n", i, pairs[i].winner, pairs[i].loser);
        // printf("binRes: %i\n", binResult);
        //  factor in the other rows. Ignore target row, so -1
        for (int j = 0; j < candidate_count - 1; j++)
        {
            binTemp = 0b0;
            checkRow++;
            if (checkRow >= candidate_count) // at row limit, wrap around.
            {
                checkRow = 0;
            }
            // printf("Row: %i - ", checkRow);

            // for the row, get all the col values
            for (int k = 0; k < candidate_count; k++)
            {
                if (locked[checkRow][k] == true)
                {
                    colVal = 1;
                }
                else
                {
                    colVal = 0;
                }
                binTemp = (binTemp << 1) | colVal; // build e.g. 011
            }
            // printf("binTemp: %i\n", binTemp);
            // printf("before Result: %d\n", binResult);
            //  then OR it with the result
            binResult = binResult | binTemp;
            //printf("after Result: %d\n\n", binResult);
        }

        // printf("Final Result: %d\n\n", binResult);
        if (binResult != cycleVal)
        {
            // printf("IN: %i\n\n", cycleVal);
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
*/

// Print the winner of the election
/*
eg 1
010
001
000

0 -> 1
1 -> 2

winner would be:
0

eg 2
010
000
100

0 -> 1
2 -> 0

*/
void print_winner(void)
{
    // TODO
    int counter;
    for (int i = 0; i < candidate_count; i ++)
    {
        counter = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                // By checking the column of the current locked pair, you are checking whether or not other candidates have an
                // edge pointing at you.
                counter++;
            }
        }

        if (counter == candidate_count)
        {
            // Due to lock_pair removing cyclical edges, the candidate with no edges pointing at it
            // (meaning all other values in the col are false, counter == candidate_count) is the source.
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

// ./tideman a b c

/*
test for cycle

./tideman a b c
3

a b c
b c a
c a b

0 -> 1
1 -> 2
2 -> 0

--
./tideman a b c
4

a b c
b c a
c a b
c a b

010
000
100

2 -> 0
0 -> 1

winner 2


./tideman a b c d < test4.txt
*/

/*

Be sure to test your code to make sure it handles…

An election with any number of candidate (up to the MAX of 9)
Voting for a candidate by name
Invalid votes for candidates who are not on the ballot
Printing the winner of the election
Execute the below to evaluate the correctness of your code using check50. But be sure to compile and test it yourself as well!

check50 cs50/problems/2023/x/tideman


-----
Execute the below to evaluate the style of your code using style50.

style50 tideman.c

*/