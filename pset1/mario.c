// Mario: receive integer height from user, then output a pyramid of that height.

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n;
    do
    {
        printf("Height: ");
        n = get_int();
    }
    while (n < 0 || n > 23);
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        
        printf("  ");
      
        for (int l = 0; l < i + 1; l++)
        {
            printf("#");
        }
        
        printf("\n");
    }
    
    return 0;
}
