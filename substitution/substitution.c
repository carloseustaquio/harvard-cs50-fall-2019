#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    string str = argv[1];
    bool alpha = true;
    bool repeated = false;

    // if no argument, returns error message
    if (str == NULL)
    {
        printf("Usage ./substitution key.\n");
        return 1;
    }

    // if the argument doesn't has 16 characters
    // returns error message
    if (strlen(str) < 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // loops through str checking if every char is
    // alphabetical
    for (int i = 0; i < strlen(str); i++)
    {
        if (isalpha(str[i]) == false)
        {
            alpha = false;
        }
    }
    if (alpha == false)
    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }

    // loops through str twice checking if
    // there's no repeated char
    for (int i = 0; i < strlen(str); i++)
    {
        int repeat = 0;
        for (int j = 0; j < strlen(str); j++)
        {
            if (str[i] == str[j])
            {
                repeat++;
            }
        }

        if (repeat > 1)
        {
            repeated = true;
        }
    }

    if (repeated == true)
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }

    // gets user input
    string text = get_string("plaintext: ");
    int tex_len = strlen(text);
    char ciphertext[tex_len];

    // loops through user input encoding
    // every char by its type, uppercase
    // or lowercase.
    for (int i = 0; i < tex_len; i++)
    {
        if (isalpha(text[i]) == 0)
        {
            ciphertext[i] = text[i];
        }

        if (islower(text[i]) != 0)
        {
            int index = (int) text[i] - 97; // a == 97
            ciphertext[i] = tolower(str[index]);
        }

        else if (isupper(text[i]) != 0)
        {
            int index = (int) text[i] - 65; // a == 65
            ciphertext[i] = toupper(str[index]);
        }

    }

    // adds null terminator to ciphertext string
    ciphertext[tex_len] = '\0';

    // prints out the result
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}