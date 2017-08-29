/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int bot = 0;
    int mid = 0;
    
    // while (sub)array is greater than size 0
    while (n - bot >= 0)
    {
        // find midpoint
        mid = (bot + n) / 2;
        
        // if midpoint is the value, return true
        if (values[mid] == value)
        {
            return true;
        }
        
        // if midpoint lower than value, check right
        if (values[mid] < value)
        {
            bot = mid + 1;
        }
        
        // if midpoint higher than value, check left
        if (values[mid] > value)
        {
            n = mid - 1;
        }
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // declare minimum indexing integer
    int min;
    
    // iterate through array
    for (int i = 0; i < n - 1; i++)
    {
        // declare i as minimum integer
        min = i;
        
        // if values[j] is lower than values[min], min takes on the new lower valued element index
        for (int j = i + 1; j < n; j++)
        {
            if (values[min] > values[j])
            {
                min = j;
            }
        }
        
        // if i was not the true minimum of array, swap values[i] and values[min]
        
        if (min != i)
        {
            int swap = values[i];
            values[i] = values[min];
            values[min] = swap;
        }
        
    }
    return;
}
