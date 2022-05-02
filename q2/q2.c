#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>

// threadc functions
void *multiplyP1(void *);
void *addWithbasis(void *);

// the constants that will make matrices
#define weightMR 3
#define weightMC 4
#define featureMC 1

// all other constants can be derived from the all above

// defining the matrices that wil be used in the process ---- memory allocation
double weightMatrix[weightMR][weightMC] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // the min size
double featureMatrix[weightMC][featureMC] = {1, 1, 1, 1};                       // multipliction is when when column of first row of second are same
double mullResult[weightMR][featureMC] = {};                                    // the resultant will be this
double basisMatrix[weightMR][featureMC] = {1, 1, 1};                            // bsis must be of that size for the addition
double resultantMatrix[weightMR][featureMC];                                    // addition resultant
double sigmoidMatrix[weightMR][featureMC];                                      // the sigmoid applieds

int main()
{

    double *returnedFromthread;

    srand(time(NULL));

    // --------------------------------------------------------------------------------
    // ---------------------------- Random Initalization ------------------------------
    // --------------------------------------------------------------------------------

    // // random initalization of required matrices
    // for (int i = 0; i < weightMR; i++)
    // {
    //     for (int j = 0; j < weightMC; j++)
    //     {
    //         weightMatrix[i][j] = rand() % 673 - 200.56;
    //     }
    // }

    // // initalizing the feature matrix
    // for (int i = 0; i < weightMC; i++)
    // {
    //     for (int j = 0; j < featureMC; j++)
    //     {
    //         featureMatrix[i][j] = rand() % 300 - 150.33;
    //     }
    // }

    // initalizing the basis matrix
    for (int i = 0; i < weightMR; i++)
    {
        for (int j = 0; j < featureMC; j++)
        {
            basisMatrix[i][j] = rand() % 300 - 150.33;
        }
    }

    // the id's off functions
    pid_t prcssId1, prcssId2, prcssId3;

    // making the three childs and will wait for ones response then will create other and in each process will do multi threading
    prcssId1 = fork();

    if (prcssId1 == 0)
    {
        // child
        // will do now multi threading --- such that
        // thredas created wll be equal to the number of the rows of weight matrix thus making a array for the thrread id's
        pthread_t threadIds[weightMR];

        int j = 0;

        // creating that much threads and performing the multiplication in a loop
        for (int i = 0; i < weightMR; i++)
        {
            j = 0;

            pthread_create(&threadIds[i], NULL, multiplyP1, (void *)&i); // passing the row as the argument for which thread will do multiplication
            pthread_join(threadIds[i], (void *)&returnedFromthread);     // waiting for the thread and storing the multiplied result by thread in the resiltant matrix

            // if multiple column output of multiplication
            while (j < featureMC)
            {
                mullResult[i][j] = *(double *)returnedFromthread;
                (double *)returnedFromthread++;
                j++;
            }
        }

        // writing this matix output on pipe and will read from that
        char Result[500];
        char convert[5];


        // till now the multiplication has been done going for the next step
    }
    else
    {

        // parent
        wait(NULL); // waiting for the first p1

        // making the other process
        prcssId2 = fork();

        if (prcssId2 == 0)
        {
            // child 2
            // this is p2 will do multithreding here also
            // such that threds equal to number of rows of basis
            // for holding threads ids
            pthread_t threadIds[weightMR];

            // creating the threads
            for (int i = 0; i < weightMR; i++)
            {
                pthread_create(&threadIds[i], NULL, addWithbasis, (void *)&i); // sending row  as the perameter as to add this row
                pthread_join(threadIds[i], NULL);
            }
        }

        // wait till the p2 finishes
        wait(NULL);

        // making the other process
        prcssId3 = fork();

        if (prcssId3 == 0)
        {
            // child
        }
    }

    return 0;
}

// definitions of functions
void *multiplyP1(void *args)
{
    // since the matrices to multiply we have
    // decoding the row number to multiply
    int *row = (int *)args;

    double *result = malloc((1 * featureMC) * sizeof(double));

    for (int i = 0; i < featureMC; i++)
    {
        for (int j = 0; j < weightMC; j++)
        {
            result[i] = result[i] + (*weightMatrix[*row] * featureMatrix[i][j]);
            *row++;
        }
    }

    return (void *)result; // returing the obtained row
}

// the addition of resultant from p1 with the basis
void *addWithbasis(void *args)
{

    // decoding the row number to multiply
    int *row = (int *)args;

    // lokking for the columns in the particular row
    double *result = malloc((1 * featureMC) * sizeof(double));

    for (int i = 0; i < featureMC; i++)
    {

        result[i] = mullResult[*row][i] + basisMatrix[*row][i];

        // printf("\n%f : %f\n", mullResult[*row][i], basisMatrix[*row][i]);
    }

    return NULL;
}