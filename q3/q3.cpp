#include <iostream>
#include <queue>

using namespace std;

// structures that will be used
// the incoming task
struct Task
{
};

// the completed task
struct CompletedTask
{
};

// ------------------------------------------------------
// -------- The memory accessed by all threads ----------
// ------------------------------------------------------
// making the global queues

queue<Task> waitingQueue;             // from the reception
queue<CompletedTask> finishedWaiting; // the completed to go to print

// -----------------------------------------------------
// ---------------- Main Threads -----------------------
// -----------------------------------------------------

void *reception(void *);
void *dispatcher(void *);
void *print(void *);

// -----------------------------------------------------
// ------------ Dispatcher Threads ---------------------
// -----------------------------------------------------

void *unit0(void *);
void *unit1(void *);
void *unit2(void *);
void *unit3(void *);
void *unit4(void *);

// ----------------------------------------------------
// ----------------------------------------------------
// ----------------------------------------------------

int main()
{

    return 0;
}