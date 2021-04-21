// Implements a phone book without structs

#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    string name;
    string phone;
}
person;

int main(void)
{
    person people[4];

    people[0].name = "Carlos";
    people[0].phone = "9981-1530";

    people[1].name = "Junior";
    people[1].phone = "9280-9283";

    people[2].name = "Mariana";
    people[2].phone = "2433-8379";

    people[3].name = "Joao";
    people[3].phone = "4234-2343";

    for (int i = 0; i < 4; i++)
    {
        if (!strcmp(people[i].name, "Carlos"))
        {
            printf("Found %s\n", people[i].phone);
            return 0;
        }
    }
    printf("Not found\n");
    return 1;
}