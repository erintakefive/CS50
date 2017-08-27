// Credit: receive credit card number as input, then check if that number is valid.

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    long long num;
    do
    {
        printf("Number: ");
        num = get_long_long();
    }
    while (num < 0);
    
    int x = 0;
    int a = 0;
    int b = 0;
    
    long long num2 = num;
    
    for (int i = 0; i < 16; i++)
    {
        if(i % 2 == 0)
        {
            a = num % 10;
            x += a;
            num = num / 10;
        }
        else
        {
            b = num % 10;
            b *= 2;
            while (b > 9)
            {
                b -= 9;
            }
            x += b;
            num = num / 10;
        }
    }
    
    if(x % 10 == 0)
    {
        if(num2 >= 1000000000000000)
        {
            num2 /= 100000000000000;
            if(num2 == 51 || num2 == 52 || num2 == 53 || num2 == 54 || num2 == 55)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                if((num2 /= 10) == 4)
                {
                    printf("VISA\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
        }
        else
        {
            if(num2 >= 100000000000000)
            {
                num2 /= 10000000000000;
                if(num2 == 34 || num2 == 37)
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            else
            {
                if(num2 >= 1000000000000)
                {
                    num2 /= 1000000000000;
                    if(num2 == 4)
                    {
                        printf("VISA\n");
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                }
                else
                {
                    printf("INVALID\n");
                }
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }
   
    return 0;
    
}
