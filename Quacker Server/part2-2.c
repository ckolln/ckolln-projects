/*
Description: Creates the proxy thread pool
Author: Claire Kolln
Notes: This file should compile with "make", execute to see the test results

 */
/*-------------------------Preprocessor Directives---------------------------*/
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/time.h>           // for gettimeofday()
#include <time.h>      
#include <pthread.h>  
#include <string.h>
#include <unistd.h>  
#include <stdlib.h>
#include <sched.h>

#define MAXENTRIES 3
#define MAXTOPICS 1
#define URLSIZE 25
#define CAPSIZE 20
#define MAXNAME 10
// #define MAXQUEUES 1
#define DELTA 3
// #define MAXPUBS 4
// #define MAXSUBS 4
#define NUMPROXIES 2
#define NUMPUBTASKS 3
#define NUMSUBTASKS 1

/*---------------------------------------------------------------------------*/
/*-----------------------------Declarations----------------------------------*/
/* Variables */

/* Structs */
struct topicEntry;
struct topicQueue;
struct timeval;
/* Functions */
// int gettimeofday(struct timeval *tv, struct timezone *tz);
// int getEntry(int lastEntry, topicEntry *empty, char *ID);
// int enqueue(topicEntry *entry, char *ID);
// int dequeue(char *ID);


// void timersub(struct timeval *a, struct timeval *b, struct timeval *res);

// int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
// int pthread_mutex_lock(pthread_mutex_t *mutex);         // Locks a mutex object
// int pthread_mutex_unlock(pthread_mutex_t *mutex);       // Releases a mutex object
void *publisher(void *args);
void *subscriber(void *args);
void *cleaner(void *args);

/*---------------------------------------------------------------------------*/
/*----------------------Struct/Enumeration Definitions-----------------------*/

typedef struct {
    int entryNum;               // unique numerical identifier for each topic entry
    struct timeval timeStamp;
    int pubID;
    char photoURL[URLSIZE];
    char photoCaption[CAPSIZE];
} topicEntry;

typedef struct {
    char *name;
    int counter;
    topicEntry * const buffer;
    int head;
    int tail;
    const int length;
} topicQueue;

typedef struct {
    int threadId;
    int free;
    int topicId;
    topicEntry pubtasks[NUMPUBTASKS];
} pArgs;

typedef struct {
    int threadId;
    int lastEntry;
    int free;
    int subtasks[NUMSUBTASKS];
} sArgs;

#define TQ_DEF(x,n,l)                   \
    topicEntry x##_buffer[MAXENTRIES+1];   \
    topicQueue x = {                    \
        .name = n,                      \
        .counter = 0,                   \
        .buffer = x##_buffer,           \
        .head = 0,                      \
        .tail = 0,                      \
        .length = l                     \
    }

/*---------------------------------------------------------------------------*/
/*--------------------------Function Definitions-----------------------------*/

topicQueue *store[MAXTOPICS];

// topicEntry pubtasks[NUMPUBTASKS];              // an array of type topicEntry
// int subtasks[NUMSUBTASKS];                    // an array of integers which correspond to topic queue

pArgs pubargs[NUMPROXIES];
sArgs subargs[NUMPROXIES];

pthread_t pubs[NUMPROXIES];
pthread_t subs[NUMPROXIES];
pthread_mutex_t mutex[MAXTOPICS];
pthread_mutex_t proxy_mutex = PTHREAD_MUTEX_INITIALIZER;

// condition var
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char *topics[] = {"dogs","cats","horses", "bugs","snakes"};


int enqueue(topicEntry *entry, int q) {
    // do mutex stuff in here
    if(store[q]->buffer[(store[q]->head +1)].entryNum == -1) {
        //printf("ENQUEUE: queue full\n");
        // the queue is full, make the thread wait until there is a dequeue
        return 0;
    }
    else {
        // set the topic number 
        int c = (store[q]->counter) + 1;
        // printf("Topic Number is: %d\n",c);
        store[q]->counter++;
        entry->entryNum = c;

        // printf("Entry entryNum is: %d\n",entry->entryNum);
        // set the timestamp
        struct timeval stamp;
        gettimeofday(&stamp,NULL);
        entry->timeStamp = stamp;

        // printf("Timestamp seconds of enqueue: %ld\n",stamp.tv_sec);
        // printf("Timestamp seconds of timeStamp entry: %ld\n",entry->timeStamp.tv_sec);

        // if the entry isn't going to be the first in the buffer
        if (!((store[q]->head == store[q]->tail) && (store[q]->buffer[store[q]->head].entryNum == 0))){
            // printf("INSIDE RIGHT THING\n");
            if (store[q]->head == (store[q]->length)) {
                // if head is currently at the top of the buffer, new head val = 0
                store[q]->head = 0;
            }
            else {
                store[q]->head++;
            }
        }
        store[q]->buffer[store[q]->head] = *entry;
        // printf("Timestamp seconds of timeStamp after added to the buffer: %ld\n",store[q]->buffer[store[q]->head].timeStamp.tv_sec);
        return 1;
    }
}

int getEntry(int lastEntry, topicEntry *empty, int q) {

    if ((store[q]->head == store[q]->tail) && (store[q]->buffer[store[q]->head].entryNum == 0)) {
        // the queue is empty, nothing to get
        return 0;
    }
    
    int y = store[q]->tail;
    while (store[q]->buffer[y].entryNum != -1) {
        if (store[q]->buffer[y].entryNum == (lastEntry + 1)) {
            // found next topic
            empty->entryNum = store[q]->buffer[y].entryNum;
            empty->timeStamp = store[q]->buffer[y].timeStamp;
            empty->pubID = store[q]->buffer[y].pubID;
            strcpy(empty->photoURL,store[q]->buffer[y].photoURL);
            strcpy(empty->photoCaption, store[q]->buffer[y].photoCaption);
            return 1;
        }
        else if (store[q]->buffer[y].entryNum > (lastEntry + 1)) {
            // next topic found had a entryNum greater than last
            empty->entryNum = store[q]->buffer[y].entryNum;
            empty->timeStamp = store[q]->buffer[y].timeStamp;
            empty->pubID = store[q]->buffer[y].pubID;
            strcpy(empty->photoURL,store[q]->buffer[y].photoURL);
            strcpy(empty->photoCaption,store[q]->buffer[y].photoCaption);
            return store[q]->buffer[y].entryNum;
        }

        if (y == store[q]->length) {
            y = 0;
        }
        else {
            y++;
        }
    }
    // if we have gotten  here, we looked through the entire queue, didn't find the topic, and didn't find an entryNum > than the topic
    return 0;
}

int dequeue(int q) {
    if ((store[q]->head == store[q]->tail) && (store[q]->buffer[store[q]->head].entryNum == 0)) {
        // the queue is empty, nothing to dequeue
        return -1;
    }
    
    // get current time value
    struct timeval now;
    gettimeofday(&now,NULL);
    struct timeval result;
    
    // subtract timestamp of entry at tail from current timestamp
    timersub(&now,&(store[q]->buffer[store[q]->tail].timeStamp),&result);

    int removed = 0;
    topicEntry null_entry = { .entryNum = -1 };
    topicEntry non_null_entry = { .entryNum = 0 };
    while(result.tv_sec >= DELTA) {
        
        removed = 1;
       
        // dequeue what is at tail
        store[q]->buffer[store[q]->tail] = null_entry;
        
        // in all three statements, must check to make sure head and tail are both incremented if they are equal
        if (store[q]->tail == 0) {
            // the null is at the store[q]->length index, tail is incremented by one
            store[q]->buffer[store[q]->length] = non_null_entry;
            if (store[q]->tail == store[q]->head){
                store[q]->head++;
            }
            store[q]->tail++;
        }
        else if (store[q]->tail == store[q]->length){
            // null is at tail - 1, but tail is incremented to front of buffer (index 0)
             store[q]->buffer[store[q]->tail - 1] = non_null_entry;
            if (store[q]->tail == store[q]->head){
                store[q]->head = 0;
            }
             store[q]->tail = 0;
        }
        else {
            // then the null is at the store[q]->tail - 1 index, and tail is incremented by one normally
            store[q]->buffer[store[q]->tail - 1] = non_null_entry;
            if (store[q]->tail == store[q]->head){
                store[q]->head++;
            }
            store[q]->tail++;
        }

        // check if next oldest item in queue also needs to be removed        
        if (store[q]->buffer[store[q]->tail].entryNum > 0) {
            gettimeofday(&now,NULL);
            timersub(&now,&(store[q]->buffer[store[q]->tail].timeStamp),&result);
        }
        else {
            break;
        }
    }
    if (removed) {
        // if you dequeued anything
        return 1;
    }
    // if you didn't dequeue anything
    return 0;
}

void *publisher(void *args) {
    int x = ((pArgs *)args)->topicId;
    int id = ((pArgs *)args)->threadId-1;
    while(1) {
        pthread_mutex_lock(&proxy_mutex);
        while(((pArgs *)args)->free) {
            // printf("Publisher proxy waiting\n");
            pthread_cond_wait(&cond, &proxy_mutex);
        }
        pthread_mutex_unlock(&proxy_mutex);
        // for all the entries it was given to publish: publish them 
        for (int z = 0; z < NUMPUBTASKS; z++) {
            topicEntry enter = ((pArgs *)args)->pubtasks[z];
            while(1) {
            // if the thread is not free: it should be running until there are no more entries for it to publish
                    pthread_mutex_lock(&(mutex[x]));
                    // printf("Pub #%d: Aquired mutex\n",((pArgs *)args)->threadId);
                    int y = enqueue(&enter,x);
                    if (!y) {
                        // printf("Pub #%d: Waiting for cleaner\n",((pArgs *)args)->threadId);
                        pthread_mutex_unlock(&(mutex[x]));
                        sched_yield();
                    }
                    else {
                        printf("Pub Thread #%d: Added new entry to topic thread w/ caption: %s\n",((pArgs *)args)->threadId,enter.photoCaption);
                        pthread_mutex_unlock(&(mutex[x]));
                        // because we have now added that topicEntry, we break from the while loop
                        break;
                    }
                }
        }
        printf("Pub Thread %d: All entries have been published, freeing thread\n\n",((pArgs *)args)->threadId);
        ((pArgs *)args)->free = 1;
    }
    
}

void *subscriber(void *args) {
    int returned;
    int x, q;
    int last;
    // where you are going to call getEntry with mutex locks
    // if the thread is not free: it should be running until there are no more entries for it to publish
    while(1) {
        
        pthread_mutex_lock(&proxy_mutex);
        while(((sArgs *)args)->free) {
            // printf("Subscriber proxy thread is waiting\n");
            pthread_cond_wait(&cond,&proxy_mutex);
        }
        pthread_mutex_unlock(&proxy_mutex);
        
        int done = 0;

        for (x = 0; x < NUMSUBTASKS; x++) {
            q = ((sArgs *)args)->subtasks[x];
            last = 0;
            while(1) {
                // get the index of the correct topic from the topic store and assign it to q
                pthread_mutex_lock(&(mutex[q]));
                // printf("Sub #%d: aquired mutex\n",((sArgs *)args)->threadId);
                topicEntry test;
                // last = ((sArgs *) args)->lastEntry;
                // printf("Last accessed entry by thread %d was %d\n",((sArgs *)args)->threadId,((sArgs *) args)->lastEntry );
                returned = getEntry(last,&test,q);

                if (returned > 1) {
                    printf("Sub Thread #%d: Original entry was dequeued, found next entry w caption: %s\n",((sArgs *)args)->threadId,test.photoCaption);
                    last = returned;
                    pthread_mutex_unlock(&(mutex[q]));
                }
                else if (returned == 0) {
                    printf("Sub Thread #%d: Queue is empty or entry is not queued yet. Freeing Thread\n",((sArgs *)args)->threadId);
                    // unlocks mutex for q, and breaks
                    pthread_mutex_unlock(&(mutex[q]));
                    break;
                }
                else {
                    printf("Sub Thread #%d: Found next entry w/ caption: %s\n",((sArgs *)args)->threadId,test.photoCaption);
                    last++;
                    pthread_mutex_unlock(&(mutex[q]));
                    // sleep(1);
                }
            }
        }
        ((sArgs *)args)->free = 1;
    }
}

void *cleaner(void *args) {
    int v;
    // where you are going to call dequeue with mutex locks round robin style on all the queues
    while(1) {
        for (int q = 0; q < MAXTOPICS; q++) {
            pthread_mutex_lock(&(mutex[q]));
            v = dequeue(q);
            if (v == 1) {
                printf("Dequeued from topic %d\n",q);
            }
            // else if (v == -1) {
            //     printf("DEQUEUE: found empty queue\n");
            // }
            // else {
            //     printf("DEQUEUE: found nothing old enough to dequeue\n");
            // }
            pthread_mutex_unlock(&(mutex[q]));
            sched_yield();
        }
    }
}
/*---------------------------------------------------------------------------*/
/*-----------------------------Program Main----------------------------------*/

int main() {
    int c;
    topicEntry last = { .entryNum = -1, };
    
    pthread_attr_t attr;                    // thread attributes
    pthread_attr_init(&attr);               //initializes the thread attributes object pointed to by attr with default attribute values.

    printf("------------------------------Test for part2 starts---------------------------------\n");

    TQ_DEF(q1,topics[0],MAXENTRIES);
    printf("Name: %s, Head: %d, Tail: %d, Length: %d\n",q1.name,q1.head,q1.tail,q1.length);
    q1.buffer[MAXENTRIES] = last;
    printf("Null topic entryNumber: %d\n",q1.buffer[MAXENTRIES].entryNum);
    store[0] = &q1;
    store[0]->buffer[0].entryNum = 0;
    // TQ_DEF(q2,topics[1],MAXENTRIES);
    // printf("Name: %s, Head: %d, Tail: %d, Length: %d\n",q2.name,q2.head,q2.tail,q2.length);
    // q2.buffer[MAXENTRIES] = last;
    // printf("Null topic entryNumber: %d\n",q2.buffer[MAXENTRIES].entryNum);
    // store[1] = &q2;

    // TQ_DEF(q3,topics[2],MAXENTRIES);
    // printf("Name: %s, Head: %d, Tail: %d, Length: %d\n",q3.name,q3.head,q3.tail,q3.length);
    // q3.buffer[MAXENTRIES] = last;
    // printf("Null topic entryNumber: %d\n",q3.buffer[MAXENTRIES].entryNum);
    // store[2] = &q3;

    // initialize mutex locks for all buffers
    for (int k = 0; k < MAXTOPICS; k++) {
        pthread_mutex_init(&mutex[k],NULL);
        printf("Queue %d name: %s\n",k, store[k]->name);
    }

    //create publisher threads, place in publisher proxy pool
    for (int i = 0; i < NUMPROXIES; i++) {
        printf("Spawning publisher proxy\n");
        pubargs[i].threadId = i + 1;                                            // assign each thread in the pool an ID
        pubargs[i].free = 1;                                                    // each thread in the pool is initially free
        c = pthread_create(&pubs[i],&attr,publisher, (void*) &pubargs[i]);
        if (c) {
            printf("Problem creating publisher proxy\n");
        }
    }

    //create subscriber threads
    for (int i = 0; i < NUMPROXIES; i++) {
        printf("Spawning subscriber proxy\n");
        subargs[i].threadId = i + 1;                                            // assign each thread in the pool an ID
        subargs[i].lastEntry = 0;
        subargs[i].free = 1;                                                    // each thread in the pool is initially free
        c = pthread_create(&subs[i],&attr,subscriber, (void*) &subargs[i]);
        if (c) {
            printf("Problem creating subscriber proxy\n");
        }
    }

    //create cleaning threads
    pthread_t clean;
    c = pthread_create(&clean, &attr,cleaner, (void*) NULL );
    if (c) {
        printf("Problem creating cleaner thread\n");
    }

    //test 1- have a subscriber thread try to read from an empty topic queue
    //allocate a subtasks array of topics that the thread should read from 

    printf("Test 1: Allocating 1 subscription proxy: should immediately return because queues are empty\n");
    subargs[0].subtasks[0] = 0;
    subargs[0].free = 0;
    
    pthread_mutex_lock(&proxy_mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&proxy_mutex);

    // test 2 - have a publisher thread fill the queue
    topicEntry test1 = { .photoCaption = "Munsterlander"};
    topicEntry test2 = { .photoCaption = "Jack Russel"};
    topicEntry test3 = { .photoCaption = "Retriever"};
    pubargs[0].pubtasks[0] = test1;
    pubargs[0].pubtasks[1] = test2;
    pubargs[0].pubtasks[2] = test3;

    printf("Test 2: Allocating 1 publisher proxy: should fill queue and return\n");
    pubargs[0].topicId = 0;
    pubargs[0].free = 0;
    pthread_mutex_lock(&proxy_mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&proxy_mutex);


    // test 3 - have a subscriber thread try to read from the topic queue
    printf("Test 3: Allocating a subscriber proxy to read from dog topic queue\n\n");
    subargs[0].subtasks[0] = 0;
    subargs[0].free = 0;
    pthread_mutex_lock(&proxy_mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&proxy_mutex);

    // test 4 - have two new publisher threads add items at the same time
    topicEntry test4 = { .photoCaption = "Puppy"};
    topicEntry test5 = { .photoCaption = "poodle"};
    topicEntry test6 = { .photoCaption = "pug face"};

    topicEntry test7 = { .photoCaption = "Labradore"};
    topicEntry test8 = { .photoCaption = "Licking faces"};
    topicEntry test9 = { .photoCaption = "Laughter"};

    pubargs[0].pubtasks[0] = test4;
    pubargs[0].pubtasks[1] = test5;
    pubargs[0].pubtasks[2] = test6;

    pubargs[1].pubtasks[0] = test7;
    pubargs[1].pubtasks[1] = test8;
    pubargs[1].pubtasks[2] = test9;

    pubargs[0].topicId = 0;
    pubargs[1].topicId = 0;


    pubargs[0].free = 0;
    pubargs[1].free = 0;

    pthread_mutex_lock(&proxy_mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&proxy_mutex);



    // test 5 - have a subscriber thread try to read from the topic queue
   
    subargs[0].subtasks[0] = 0;
    subargs[0].free = 0;
    pthread_mutex_lock(&proxy_mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&proxy_mutex);

    sleep(6);
    printf("------------------------------Test for part2 ends---------------------------------\n");
    exit(0);
    return 1;
}
/*-----------------------------Program End-----------------------------------*/
