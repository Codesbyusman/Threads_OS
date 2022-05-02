#include <iostream>

using namespace std;

// making the structure of the item
struct Item
{
    char itemName[50]; // for the name of item
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
struct Item allItems[allItemsize] = {
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

int main()
{

    
    return 0;
}
