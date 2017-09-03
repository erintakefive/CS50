/**
 * collatz.c contains a recursive function 'collatz()' that
 * tracks the amount of steps it takes for an integer (in
 * this case the atoi of argv[1]) to reach 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int collatz(int n);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./collatz (Number)");
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    int steps = collatz(n);

    printf("Steps: %i\n", steps);

    return 0;
}

int collatz(int n)
{
    printf("n = %i\n", n);
    
    if (n == 1)
    {
        return 0;
    }
    else 
    {
        if ((n % 2) == 0)
        {
            return 1 + collatz(n / 2);
        }
        else
        {
            return 1 + collatz((3*n) + 1);
        }
    }
}
