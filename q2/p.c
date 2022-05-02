#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>

int main()
{

    double a = 9.5;

    printf("\n%f\n", a);

    char c[5];
    itoa(a, c, 10);

    printf("\n%s\n", c);
    return 0;
}