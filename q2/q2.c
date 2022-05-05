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

// for printing the matrices
void printMatrix(double array[][featureMC])
{

    // the upper
    printf("\n\t  __");

    for (int i = 0; i <= featureMC; i++)
    {
        printf("\t ");
    }

    printf("\b\b\b __\n");

    printf("\t |");

    for (int i = 0; i <= featureMC; i++)
    {
        printf("\t ");
    }

    printf("\b\b  |\n");

    // the main content
    for (int i = 0; i < weightMR; i++)
    {
        printf("\t | ");
        for (int j = 0; j < featureMC; j++)
        {

            printf("%8.2f", array[i][j]);
        }

        printf("\t | \n");
    }

    // the lower

    printf("\t |_");

    for (int i = 0; i <= featureMC; i++)
    {
        printf("\t ");
    }

    printf("\b\b _|\n\n");
}

// for printing the weight matrix
void printWeightMatrix(double array[][weightMC])
{

    // the upper
    printf("\n\t  __");

    for (int i = 0; i <= weightMC; i++)
    {
        printf("\t ");
    }

    printf("\b\b\b __\n");

    printf("\t |");

    for (int i = 0; i <= weightMC; i++)
    {
        printf("\t ");
    }

    printf("\b\b  |\n");

    // the main content
    for (int i = 0; i < weightMR; i++)
    {
        printf("\t | ");
        for (int j = 0; j < weightMC; j++)
        {

            printf("%8.2f", array[i][j]);
        }

        printf("\t | \n");
    }

    // the lower

    printf("\t |_");

    for (int i = 0; i <= weightMC; i++)
    {
        printf("\t ");
    }

    printf("\b\b _|\n\n");
}

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
    // for (int i = 0; i < weightMR; i++)
    // {
    //     for (int j = 0; j < featureMC; j++)
    //     {
    //         basisMatrix[i][j] = rand() % 300 - 150.33;
    //     }
    // }

    //   writing the result to the pipe --- mul results
    int fdMulresult[2];
    pipe(fdMulresult);

    // writing result of addition
    int fdAddresult[2];
    pipe(fdAddresult);

    // the id's off functions
    pid_t prcssId1, prcssId2, prcssId3;

    // making the three childs and will wait for ones response then will create other and in each process will do multi threading
    prcssId1 = fork();

    if (prcssId1 == 0)
    {
         printf("\n child 1 \n");

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
        char result[500] = " ";
        char convert[10];

        for (int i = 0; i < weightMR; i++)
        {
            for (int j = 0; j < featureMC; j++)
            {
                sprintf(convert, "%.2f", mullResult[i][j]); // converting to string so to concatenate
                strcat(result, convert);                    // concatenating
                strcat(result, ":");                        // cancatinating the : with result
            }
        }

        // closing the reading end here it will write only
        close(fdMulresult[0]);
        write(fdMulresult[1], result, sizeof(result));

        // till now the multiplication has been done going for the next step
    }
    else
    {

        // parent
        wait(NULL); // waiting for the first p1

         printf("\n parrent \n");

        // will read mul result -- will close writing of that pipe here
        close(fdMulresult[1]);

        // the result containint
        char resultMull[500];
        read(fdMulresult[0], resultMull, sizeof(resultMull));

        char *pipeRead = strtok(resultMull, ":");

        // assigong the first extracted
        double resultD = strtod(pipeRead, NULL);
        mullResult[0][0] = resultD;

        // adding the result to the resultant matrix that was produced by the child 1
        for (int i = 0; i < weightMR && pipeRead != NULL; i++)
        {

            for (int j = 0; j < featureMC && pipeRead != NULL; j++)
            {
                if (!(i == 0 && j == 0))
                {
                    // breaking to the pices and writing to the desired array
                    pipeRead = strtok(NULL, ":");
                    // assigong the extracted
                    resultD = strtod(pipeRead, NULL);

                    mullResult[i][j] = resultD;
                }
            }
        }

        // making the other process
        prcssId2 = fork();

        if (prcssId2 == 0)
        {

            printf("\n child 2 \n");
            // child 2
            // this is p2 will do multithreding here also
            // such that threds equal to number of rows of basis
            // for holding threads ids
            pthread_t threadIds[weightMR];

            int j = 0;

            // creating the threads
            for (int i = 0; i < weightMR; i++)
            {
                j = 0;

                pthread_create(&threadIds[i], NULL, addWithbasis, (void *)&i); // sending row  as the perameter as to add this row
                pthread_join(threadIds[i], (void *)&returnedFromthread);       // waiting for the thread and storing the multiplied result by thread in the resiltant matrix

                // if multiple column output of multiplication
                while (j < featureMC)
                {

                    resultantMatrix[i][j] = *(double *)returnedFromthread;
                    (double *)returnedFromthread++;
                    j++;
                }
            }

            // writing this matix output on pipe and will read from that
            char resultAdd[500];
            char convertAdd[10];

            for (int i = 0; i < weightMR; i++)
            {
                for (int j = 0; j < featureMC; j++)
                {
                    sprintf(convertAdd, "%.2f", resultantMatrix[i][j]); // converting to string so to concatenate
                    strcat(resultAdd, convertAdd);                    // concatenating
                    strcat(resultAdd, ":");                        // cancatinating the : with result
                }
            }

            printf("\n%s\n ",resultAdd);

            // closing reading end here
            close(fdAddresult[0]);
            write(fdAddresult[1], resultAdd, sizeof(resultAdd));


            printMatrix(resultantMatrix);
        }

        // wait till the p2 finishes
        wait(NULL);

         printf("\n parent \n");

        // making the other process
        prcssId3 = fork();

        if (prcssId3 == 0)
        {
             printf("\n child 3 \n");

            // child
        }

        wait(NULL);
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
    }

    return (void *)result; // returing the result from thread
}
