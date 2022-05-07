/* 
        Muhammad Usman Shahid
                20i-1797
                    T
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <bits/stdc++.h>
#include <queue>
#include <math.h>
#include <iomanip>

using namespace std;

#define BILLION 1000000000L;

// structures that will be used
// the incoming task
struct Task
{
    int id;                // id of this task
    int task_value;        // value of this task
    timespec arrival_time; // the arrival time of the task
    int unit_count;        // number of units
    int *unitIdList;       // list of units which task will be assigned to
};

// the completed task
struct CompletedTask
{
    int id;                   // id of this task
    int task_value;           // value
    timespec arrival_time;    // the arrival time of the task
    timespec completion_time; // the completion time
    int unit_count;           // number of units
    int *unitIdList;          // list of units which task will be assigned to
    int *results;             // value of this task operations i.e task value 1 ...
};

// ------------------------------------------------------
// -------- The memory accessed by all threads ----------
// ------------------------------------------------------
// making the global queues

queue<Task> waitingQueue;             // from the reception
queue<CompletedTask> finishedWaiting; // the completed to go to print

// the file read completely
bool fileRead = false;
const int S = 5000;
bool dispatcherFinished = false;

// ------------- Queues for the units ------------------

queue<Task *> unitOperation0;
queue<Task *> unitOperation1;
queue<Task *> unitOperation2;
queue<Task *> unitOperation3;
queue<Task *> unitOperation4;

// the response queues for the units

// queue<int> unitOperation0Result;
// queue<int> unitOperation1Result;
// queue<int> unitOperation2Result;
// queue<int> unitOperation3Result;
// queue<int> unitOperation4Result;

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

void convertToTask(string, Task &);

// ----------------------------------------------------
// ----------------------------------------------------
// ----------------------------------------------------

int main()
{

    // the ids for the thread
    pthread_t receptionThread1, dispatcherThread2, printThread3;

    // creating the first thread
    pthread_create(&receptionThread1, NULL, reception, NULL);

    // creating the second thread
    pthread_create(&dispatcherThread2, NULL, dispatcher, NULL);

    // creating the thrid thread
    pthread_create(&printThread3, NULL, print, NULL);

    // joining so that main doesn't end before there execution
    pthread_join(receptionThread1, NULL);
    pthread_join(dispatcherThread2, NULL);
    pthread_join(printThread3, NULL);

    return 0;
}

// ----------------------------------------------------
// ---------- Threads Function Definition -------------
// ----------------------------------------------------

// --------------- The main Threads -------------------
// the reception continue till the file is not ended
void *reception(void *args)
{
    ifstream file; // the file
    string line;   // to store the line

    // the task
    Task task;

    file.open("input.txt");

    if (file.is_open())
    {
        // while not end of file
        getline(file, line, '\n'); // taking the first line and ignoring as heading

        while (!file.eof())
        {
            // while not end of file
            getline(file, line, '\n');
            convertToTask(line, task);
            clock_gettime(CLOCK_REALTIME, &task.arrival_time); // the arrival time is saved and will be used next
            waitingQueue.push(task);                           // pushing the task to the  waiting queue
        }

        file.close(); // closing the file

        fileRead = true;
    }
    else
    {
        cout << "error in opening" << endl;
    }

    return NULL;
}

void *dispatcher(void *args)
{

    pthread_t *dispatcherThreadids;

    // the task that will be taken from the queue
    Task taskTodo;

    int count = 0;

    int *unitsResults; // will contain the result of units

    // infinite loop till the queue is empty and the input file read completely
    while (1)
    {

        if (!waitingQueue.empty())
        {
            taskTodo = waitingQueue.front();
            waitingQueue.pop();

            // assiging to the units
            // assigining to the queues
            // making the completed task object
            // checking how many threads will be made
            count = taskTodo.unit_count;
            dispatcherThreadids = new pthread_t[count];
            unitsResults = new int[count]; // the list of answers from thread

            void **fromThread; // result from thread

            for (int i = 0; i < count; i++)
            {

                if (taskTodo.unitIdList[i] == 0)
                {
                    // thread creation and result taking
                    pthread_create(&dispatcherThreadids[i], NULL, unit0, (void *)&taskTodo);
                    pthread_join(dispatcherThreadids[i], (void **)&fromThread);

                    // dequeuing from the queue the entry
                    unitOperation0.pop();

                    // saving answer in the thread
                    unitsResults[i] = *(&*(int *)fromThread);
                }
                else if (taskTodo.unitIdList[i] == 1)
                {
                    // thread creation and result taking
                    pthread_create(&dispatcherThreadids[i], NULL, unit1, (void *)&taskTodo);
                    pthread_join(dispatcherThreadids[i], (void **)&fromThread);

                    // dequeuing from the queue the entry
                    unitOperation0.pop();

                    // saving answer in the thread
                    unitsResults[i] = *(&*(int *)fromThread);
                }
                else if (taskTodo.unitIdList[i] == 2)
                {
                    // thread creation and result taking
                    pthread_create(&dispatcherThreadids[i], NULL, unit2, (void *)&taskTodo);
                    pthread_join(dispatcherThreadids[i], (void **)&fromThread);

                    // dequeuing from the queue the entry
                    unitOperation0.pop();

                    // saving answer in the thread
                    unitsResults[i] = *(&*(int *)fromThread);
                }
                else if (taskTodo.unitIdList[i] == 3)
                {
                    // thread creation and result taking
                    pthread_create(&dispatcherThreadids[i], NULL, unit3, (void *)&taskTodo);
                    pthread_join(dispatcherThreadids[i], (void **)&fromThread);

                    // saving answer in the thread
                    unitsResults[i] = *(&*(int *)fromThread);
                }
                else if (taskTodo.unitIdList[i] == 4)
                {
                    // thread creation and result taking
                    pthread_create(&dispatcherThreadids[i], NULL, unit4, (void *)&taskTodo);
                    pthread_join(dispatcherThreadids[i], (void **)&fromThread);

                    // dequeuing from the queue the entry
                    unitOperation0.pop();

                    // saving answer in the thread
                    unitsResults[i] = *(&*(int *)fromThread);
                }
                else
                {
                    // invalid do nothing
                    continue;
                }
            }

            CompletedTask taskDone; // the done task

            // assigining the values
            clock_gettime(CLOCK_REALTIME, &taskDone.completion_time); // the completion time is saved and will be used next
            taskDone.id = taskTodo.id;                                // the id
            taskDone.task_value = taskTodo.task_value;                // the value of task given
            taskDone.unit_count = taskTodo.unit_count;                // the counts
            taskDone.arrival_time = taskTodo.arrival_time;            // arrival time
            taskDone.unitIdList = taskTodo.unitIdList;                // unit ids
            taskDone.results = unitsResults;                          // unit results

            // assiging this to the completed queue
            finishedWaiting.push(taskDone);
        }

        // break as dispatcher work is completed
        if (fileRead && waitingQueue.empty())
        {
            break;
        }
    }

    dispatcherFinished = true; // the all done by dispatcher

    return NULL;
}

void *print(void *args)
{

    CompletedTask done;
    double completionTime;
    string mixResult;

    cout << endl;
    cout << setw(20) << "  Task ID  |"
         << setw(25) << "  Completion Time (ms)  |"
         << setw(15) << "  Unit Id's  |"
         << setw(18) << " Task Value 1  |"
         << setw(19) << " Task Value 2  |"
         << setw(19) << " Task Value 3  |"
         << setw(19) << " Task Value 4  |"
         << setw(15) << "   Task Value 5 " << endl
         << endl;
    // the infinite loop will break when finished and all queues are empty and file is read
    while (1)
    {

        // dequeing from the finished and printing that
        if (!finishedWaiting.empty())
        {
            done = finishedWaiting.front(); // taking value
            finishedWaiting.pop();          // removing

            completionTime = (done.completion_time.tv_sec - done.arrival_time.tv_sec) + (double)(done.completion_time.tv_nsec - done.arrival_time.tv_nsec) / (double)BILLION;
            completionTime *= 1000; // in milii second

            // displaying the data
            cout << setw(15) << done.id << "    |   " << setw(13) << completionTime << "        | " << setw(3);

            // the unit ids
            for (int i = 0; i < done.unit_count; i++)
            {
                cout << done.unitIdList[i] << ",";
            }

            if (done.unit_count == 1)
                cout << "\b " << setw(7) << "         | ";
            if (done.unit_count == 2)
                cout << "\b " << setw(6) << "       | ";
            if (done.unit_count == 3)
                cout << "\b " << setw(5) << "     | ";
            if (done.unit_count == 4)
                cout << "\b " << setw(4) << "   | ";
            if (done.unit_count == 5)
                cout << "\b " << setw(3) << "| ";

            for (int i = 0; i < done.unit_count; i++)
            {
                mixResult = to_string(done.unitIdList[i]) + "-" + to_string(done.results[i]); // the concatenated result

                cout << setw(12) << mixResult;
                if (i != done.unit_count - 1)
                    cout << setw(5) << "    |  ";
            }
            cout << endl
                 << endl;

            // deleting the dynamic memories
            delete[] done.unitIdList;
            delete[] done.results;
        }

        // break as dispatcher work is completed
        if (fileRead && waitingQueue.empty() && dispatcherFinished && finishedWaiting.empty())
        {
            break;
        }
    }

    return NULL;
}

// ------------ The dispatcher threads ----------------

void *unit0(void *args)
{
    // decoding the rgument
    Task *task = (Task *)args;

    int *result = new int;

    // queuing the task
    unitOperation0.push(task);

    // performing the whole task
    *result = (task->task_value + 5) % S;

    // returning the value
    return (void *)result;
}

void *unit1(void *args)
{
    // decoding the rgument
    Task *task = (Task *)args;

    int *result = new int;

    // queuing the task
    unitOperation1.push(task);

    // performing the whole task
    *result = (task->task_value * 7) % S;

    // returning the value
    return (void *)result;
}

void *unit2(void *args)
{
    // decoding the rgument
    Task *task = (Task *)args;

    int *result = new int;

    // queuing the task
    unitOperation2.push(task);

    // performing the whole task
    *result = ((task->task_value * task->task_value * task->task_value * task->task_value * task->task_value) % S);

    // returning the value
    return (void *)result;
}

void *unit3(void *args)
{
    // decoding the rgument
    Task *task = (Task *)args;

    int *result = new int;

    // queuing the task
    unitOperation3.push(task);

    // performing the whole task
    *result = task->task_value - 100;

    // returning the value
    return (void *)result;
}

void *unit4(void *args)
{
    // decoding the rgument
    Task *task = (Task *)args;

    int *result = new int;

    // queuing the task
    unitOperation4.push(task);

    // performing the whole task
    *result = (task->task_value * task->task_value);

    // returning the value
    return (void *)result;
}

//  ----------------------------
void convertToTask(string line, Task &t)
{

    stringstream splitThestring(line); // will use the stream and will take one word from the given string
    string word;                       // the splted word

    // doing until getting the word
    // the first is the id thus
    splitThestring >> word;
    t.id = stoi(word); // storing the id

    // checking for the next the task value
    splitThestring >> word;
    t.task_value = stoi(word);

    // the next is unit counts
    splitThestring >> word;
    t.unit_count = stoi(word);

    // giving memory of that count
    t.unitIdList = new int[t.unit_count];

    // looping the unit count times because that much data is there
    for (int i = 0; i < t.unit_count; i++)
    {
        // the ids and there assiging
        splitThestring >> word;
        t.unitIdList[i] = stoi(word);
    }
}
