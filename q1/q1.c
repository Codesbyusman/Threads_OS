#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// making the structure of the item
struct Item
{
    char itemName[20]; // for the name of item
    double price;      // the price of the item
};

// the selected item list and size structure
struct selectedItem
{
    struct Item *selected;
    int size;          // the selected items
    int *quantityList; // the buyed
};

// making the list of the items
// such that the items in our shop --- defining the sample
// initalizig with some random data

const int allItemsize = 10;
struct Item allItems[10] = {
    {"Banana ", 20.0},
    {"Apple ", 30.0},
    {"Biryani ", 200.0},
    {"Bread ", 100.0},
    {"Chocolate ", 350.0},
    {"Egg ", 40.0},
    {"Biscuits ", 10.0},
    {"Juice ", 20.0},
    {"Chaat ", 80.0},
    {"Samoosa ", 20.0},
};

// the four thread functions
void *selectItems(void *);
void *calculateTax(void *);
void *calculateSale(void *);
void *sortPurchased(void *);

// contains the selected item
struct selctedItem *itemsPurchased;

// the main function
int main()
{
    srand(time(NULL));

    // will contain sum
    void *total, *tax, *sale;

    // id's of threads
    pthread_t id1, id2, id3, id4;

    // thread # 01
    pthread_create(&id1, NULL, selectItems, (void *)&itemsPurchased);
    pthread_join(id1, &total);

    printf("\n\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n");

    // thread # 02
    pthread_create(&id2, NULL, calculateTax, (void *)total);
    pthread_join(id2, &tax);

    printf(" \n\t\t\t:::::: Tax + Sum is : %.2f :::::: \n", *(double *)tax + *(double *)total);
    printf("\n\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n");

    pthread_create(&id3, NULL, calculateSale, (void *)total);
    pthread_join(id3, &sale);

    printf(" \n\t\t    :::::: Sum - Sale + Tax is : %.2f :::::: \n", *(double *)tax + *(double *)total - *(double *)sale);
     printf(" \n\t\t\t    :::::: Total  : %.2f :::::: \n",*(double *)tax + *(double *)total - *(double *)sale);

    printf("\n\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n");

    pthread_create(&id3, NULL, sortPurchased, (void *)&itemsPurchased);
    pthread_join(id3, &sale);

    printf("\n\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n");
    printf("\t -------------------------------------------------------------------- \n\n");

    // deallocating the memories
    free(sale);
    free(tax);
    free(total);
    free(itemsPurchased);

    return 0;
}

//  ---------- thraed 1 ---------
void *selectItems(void *args)
{

    printf("\n\t\t\t   ::::::: Thread 1 :::::: \n\n");

    // for no repipetition of items
    bool occurence[10] = {};

    double *sum = malloc(sizeof(double));

    // type casting the arguments back to real
    // will store values and will return this
    struct selectedItem *selected = (struct selectedItem *)args;

    // randomly chosing the items
    selected->size = rand() % allItemsize; // to select how many

    // allocating the dynamic memory
    selected->selected = malloc(selected->size * sizeof(struct Item));

    // also allocating radom value to the quantity
    selected->quantityList = malloc(selected->size * sizeof(int));

    int random = 0;

    // now which
    for (int i = 0; i < selected->size; i++)
    {
        do
        {
            random = rand() % 10;
        } while (occurence[random]);

        selected->selected[i] = allItems[random];
        occurence[random] = 1; // occured

        // also randomly ---- which item is bought how many time
        selected->quantityList[i] = rand() % 10 + 1;
    }

    // printing the result
    printf("\n\t Items \t  | \t  Quantity    x       Price \t | \t Total \n");

    // printing in loop
    for (int i = 0; i < selected->size; i++)
    {

        printf("\n %16s |   %10d      x     %6.2f \t |  %10.2f \n", selected->selected[i].itemName, selected->quantityList[i], selected->selected[i].price, selected->quantityList[i] * selected->selected[i].price);
        *sum += selected->quantityList[i] * selected->selected[i].price;
    }

    printf(" \n\n\t\t\t\t\t\t :::::: Sum is : %.2f :::::: \n", *sum);

    return (void *)sum; // returning nothing
}

void *calculateTax(void *args)
{

    printf("\n\t\t\t   ::::::: Thread 2 :::::: \n\n");

    // type casting the arguments back to real
    // will store values and will return this
    double *sum = (double *)args;

    double *tax = malloc(sizeof(double));

    // calculating tax
    *tax = ((*sum) * 8/ (100));
    printf(" \n\n\t\t\t    :::::: Tax is : %.2f :::::: \n", *tax);

    // returning the tax to main
    return (void *)tax;
}

void *calculateSale(void *args)
{

    printf("\n\t\t\t   ::::::: Thread 3 :::::: \n\n");

    // type casting the arguments back to real
    // will store values and will return this
    double *sum = (double *)args;

    double *sale = malloc(sizeof(double));

    // calculating sale if sum is more than 250

    if (*sum > 250.0)
    {
        *sale = 0.1 * (*sum);
    }
    else
    {
        *sale = 0;
    }

    printf(" \n\n\t\t\t    :::::: Sale is : %.2f :::::: \n", *sale);

    // returning the sale to main
    return (void *)sale;
}

void *sortPurchased(void *args)
{
    printf("\n\t\t\t   ::::::: Thread 4 :::::: \n\n");

    // type casting the arguments back to real
    // will store values and will return this
    struct selectedItem *selected = (struct selectedItem *)args;

    // the sorting factor
    double sortKey1 = 0; // will be total price
    double sortKey2 = 0; // will be  totl price that will be compared to next

    struct Item swapItem;
    int swapQunatity;

    // performing the bubble sort
    for (int i = 0; i < selected->size - 1; i++)
    {
        for (int j = 0; j < selected->size - 1 - i; j++)
        {
            sortKey1 = selected->selected[j].price * selected->quantityList[j];
            sortKey2 = selected->selected[j + 1].price * selected->quantityList[j + 1];

            if (sortKey1 < sortKey2)
            {
                // perform the swaping

                // swaping the quantities of items
                swapQunatity = selected->quantityList[j];
                selected->quantityList[j] = selected->quantityList[j + 1];
                selected->quantityList[j + 1] = swapQunatity;

                // swaping the items in list
                swapItem = selected->selected[j];
                selected->selected[j] = selected->selected[j + 1];
                selected->selected[j + 1] = swapItem;
            }
        }
    }

    // printing the result
    printf("\n\t Items \t  | \t  Quantity    x       Price \t | \t Total \n");

    // printing in loop
    for (int i = 0; i < selected->size; i++)
    {

        printf("\n %16s |   %10d      x     %6.2f \t |  %10.2f \n", selected->selected[i].itemName, selected->quantityList[i], selected->selected[i].price, selected->quantityList[i] * selected->selected[i].price);
    }

    printf("\n\n");

    return NULL;
}