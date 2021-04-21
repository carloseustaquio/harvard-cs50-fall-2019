#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check correct usage
    if (!argv[1])
    {
        printf("Correct usage ./recover + arg\n");
        return 1;
    }

    // check if only one arg
    if (argv[2])
    {
        printf("Use only one arg\n");
        return 1;
    }

    // open file
    FILE *file = fopen(argv[1], "r");

    // check if file is empty
    if (file == NULL)
    {
        printf("Couldn't open %s.\n", argv[1]);
    }

    // creates
    FILE *img = NULL;

    // buffer holds the chunk of memory that's been used in the moment
    BYTE buffer[512];

    // filename holds the name of the file that's been writing
    char filename[8];

    // flag, if a image is found sets to true, if not, to false
    bool flag = false;

    // counter of how many images were found
    int counter = 0;

    // repeats the while loop until the fread function returns 0
    while (fread(buffer, 512, 1, file) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (flag == true)
            {
                fclose(img);
            }

            else
            {
                flag = true;
            }

            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            counter++;
        }

        if (flag == true)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    fclose(file);
    fclose(img);

    return 0;
}
