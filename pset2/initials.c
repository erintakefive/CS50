// Initials: receive name from user, then output uppercase initials of that name.

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // prompt the user for a string
    string s = get_string();
    
    // ensure string s is valid
    if (s != NULL)
    {
        // for the length of s, iterate and print capitalized chars that follow '\0' (for the first letter) or ' '
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            if ((s[i-1] == '\0') || (s[i-1] == ' '))
            {
                printf("%c", toupper(s[i]));
            }
        }
        printf("\n");
    }
}
