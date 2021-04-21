#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // open file
    FILE *file = fopen("phonebook.csv", "a");

    // get strings for user
    char *name = get_string("name: ");
    char *number = get_string("number: ");

    // print strings to file
    fprintf(file, "%s,%s\n", name, number);

    // close file
    fclose(file);
}