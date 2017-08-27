// Caesar: receive input from user and encrypt, then print encoded text.

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // make sure argc is inputted correctly
    if (argc == 2)
    {   
        // convert string argv[1] key to integer
        int k = atoi(argv[1]);
        
        // make sure key = 1 to 26
        if (k > 0 && k < 26)
        {
            // get plaintext from user
            printf("plaintext: ");
            string p;
            do {
            p = get_string();
            }
            while (strlen(p) == 0);
            
            printf("ciphertext: ");
            
            // iterate over plaintext and encrypt
            for (int i = 0, n = strlen(p); i < n; i++)
            {
                if (isalpha(p[i]))
                {
                    //prepare ciphertext & alphabetical index
                    int a[n];
                    int c[n];
                    
                    // uppercase encryption
                    if (isupper(p[i]))
                    {
                        a[i] = p[i] - 65;
                        c[i] = (a[i] + k) % 26;
                        c[i] = c[i] + 65;
                        printf("%c", c[i]);
                    }
                    
                    // lowercase encryption
                    if (islower(p[i]))
                    {
                        a[i] = p[i] - 97;
                        c[i] = (a[i] + k) % 26;
                        c[i] = c[i] + 97;
                        printf("%c", c[i]);
                    }
                }
                else
                {
                    printf("%c", p[i]);
                }
            }
        }
        else
        {
            printf("Usage: k < 26\n");
            return 1;
        }
    }
    else
    {
    // prompt user for correct command line input
    printf("Usage: ./caesar k\n");
    return 1;
    }
    
    //print new line
    printf("\n");
    return 0;
}
