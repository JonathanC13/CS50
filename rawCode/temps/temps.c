// Practice working with structs
// Practice applying sorting algorithms

#include <cs50.h>
#include <stdio.h>

#define NUM_CITIES 10

typedef struct
{
    string city;
    int temp;
}
avg_temp;

avg_temp temps[NUM_CITIES];

void sort_cities(void);
void swap(int a, int b);

int main(void)
{
    temps[0].city = "Austin";
    temps[0].temp = 97;

    temps[1].city = "Boston";
    temps[1].temp = 82;

    temps[2].city = "Chicago";
    temps[2].temp = 85;

    temps[3].city = "Denver";
    temps[3].temp = 90;

    temps[4].city = "Las Vegas";
    temps[4].temp = 105;

    temps[5].city = "Los Angeles";
    temps[5].temp = 82;

    temps[6].city = "Miami";
    temps[6].temp = 97;

    temps[7].city = "New York";
    temps[7].temp = 85;

    temps[8].city = "Phoenix";
    temps[8].temp = 107;

    temps[9].city = "San Francisco";
    temps[9].temp = 66;

    sort_cities();

    printf("\nAverage July Temperatures by City\n\n");

    for (int i = 0; i < NUM_CITIES; i++)
    {
        printf("%s: %i\n", temps[i].city, temps[i].temp);
    }
}

void swap(int a, int b)
{
    avg_temp temp = temps[a];
    temps[a] = temps[b];
    temps[b] = temp;
}

// TODO: Sort cities by temperature in descending order
/*
bubble or insertion due to best case of O(n)
*/
void sort_cities(void)
{
    int i, j;
    avg_temp key;
    for (i = 1; i < NUM_CITIES; i++) {
        key = temps[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && temps[j].temp < key.temp) {
            temps[j + 1] = temps[j];
            j = j - 1;
        }
        temps[j + 1] = key;
    }
    /*
    avg_temp temp;
    // Add your code here
    // insertion sort
    for (int i = 0; i < NUM_CITIES - 1; i++)
    {
        //printf("Comp: %i, %i\n",temps[i].temp, temps[i + 1].temp);
        // if the adjacent element in the pair is greater than the current, swap, and then find the apporiate place for it.
        if (temps[i].temp < temps[i + 1].temp)
        {
            // swap
            //printf("swap\n");
            swap(i, i + 1);

            // then find the spot for the swapped elem in the 'sorted' section. Within 0 to i - 1.
            for (int j = i; j > 0; j--)
            {
                //printf("swap2: %i, %i\n", temps[j].temp, temps[j - 1].temp);
                if (temps[j].temp > temps[j - 1].temp)
                {
                    swap(j, j - 1);
                }
                else
                {
                    break; // Can break right away because the left side is considered sorted.
                }
            }
        }
    }
    */
}
