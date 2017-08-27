#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // ensure key is one word
    if (argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    
    // ensure key is alphabetical 
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }
    
    // create alphabetical index for key
    
    int key[strlen(argv[1])];
    
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isupper(argv[1][i]))
        {
            key[i] = argv[1][i] - 65;
        }
        
        if (islower(argv[1][i]))
        {
            key[i] = argv[1][i] - 97;
        }
    }
    
    // get plaintext from user
    printf("plaintext: ");
    string plaintext = get_string();
    
    printf("ciphertext: ");
    
    // create countoff variable, alphabetical index and ciphertext array
    int j = 0;
    int alpha[strlen(plaintext)];
    int ciphertext[strlen(plaintext)];
    
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // if p[i] is uppercase, convert to alpha index (-65) then add j'th element of key 
        if (isupper(plaintext[i]))
        {
            alpha[i] = plaintext[i] - 65;
            ciphertext[i] = (alpha[i] + key[j]) % 26;
            ciphertext [i] += 65;
            j++;
            printf("%c", ciphertext[i]);
        }
        
        // if p[i] is lowercase, convert to alpha index (-97) then add j'th element of key before 
        if (islower(plaintext[i]))
        {
            alpha[i] = plaintext[i] - 97;
            ciphertext[i] = (alpha[i] + key[j]) % 26;
            ciphertext [i] += 97;
            j++;
            printf("%c", ciphertext[i]);
        }
        
        // if p[i] is not alphabetical, print as normal
        if (!isalpha(plaintext[i]))
        {
            printf("%c", plaintext[i]);
        }
        
        // if final letter of key is hit, wrap around to the first letter
        if (j % strlen(argv[1]) == 0)
        {
            j = 0;
        }
    }
    
    //print new line
    printf("\n");
    return 0;
}
