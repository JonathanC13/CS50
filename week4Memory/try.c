#include <stdio.h>
#include <math.h>

void assign(int val, char *name, int paddSize)
{
    int lastIndex = paddSize - 1;

    for (int i = lastIndex; i >= 0; i--)    // - 1 for save last char for '\0'
    {
        if (val == 0)
        {
            name[i] = '0';
        }
        else 
        {
            name[i] = (val % 10) + 48;
            val = floor(val / 10);
        }
    }
}

int main(void)
{
    int val = 12;
    char c[9];

    assign(val, c, 3);

    c[8] = '\0';
    c[7] = 'g';
    c[6] = 'e';
    c[5] = 'p';
    c[4] = 'j';
    c[3] = '.';
    

    printf("%s\n", c);
}

