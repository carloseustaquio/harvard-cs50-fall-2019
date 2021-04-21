#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");

    int numL = count_letters(text);

    int numW = count_words(text);

    int numS = count_sentences(text);


    float averageL = (100 / (float) numW) * numL;

    float averageS = (100 / (float) numW) * numS;

    float index = 0.0588 * averageL - 0.296 * averageS - 15.8;

    if (index >= 16)
    {
        printf("Grade 16+\n");
        return 0;
    }

    if (index < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }

    printf("Grade %i\n", (int) round(index));
    return 0;

    // printf("%i letter(s)\n %i word(s) \n %i sentence(s) \n", numL, numW, numS);
    // printf("%f average letter(s)\n %f average sentence(s) \n", averageL, averageS);
}

int count_sentences(string text)
{
    int sentences = 0;
    // loop through all the caracters except the \0
    // every time it finds a ! or . or ? adds one to
    // the sentences variable
    for (int i = 0, n = 1; n != 0; n = text[i])
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            sentences++;
        }
        i++;
    }

    return sentences;
}

int count_words(string text)
{
    int words = 0;
    // loop through all the caracters including the \0 !!
    // every time a char is considered a space or the null
    // terminator, it ads one to the words variable
    for (int i = 0, n = 1; n != 0; i++)
    {
        if (text[i] == 32 || text[i] == 0)
        {
            words++;
        }

        n = text[i];
    }

    return words;
}

int count_letters(string text)
{
    int length = 0;
    // loop through all the caracters exept for the \0
    // every time a char is considered a alphabetic char
    // "isalpha", it adds one to the length variable
    for (int i = 0, n = 1; n != 0; n = text[i])
    {
        if (isalpha(text[i]))
        {
            length++;
        }

        i++;
    }

    return length;
}