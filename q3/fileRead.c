#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp = fopen("abc.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }
    char chunk[128];
    while (fgets(chunk, sizeof(chunk), fp) != NULL)
    {
        printf("%s", chunk);
    }

    fclose(fp);
}