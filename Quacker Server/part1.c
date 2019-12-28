/*
Description: The Quaker Topic Store. This program implements a circular
ring buffer to store published photos.
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
#define MAXNAME 25
#define DELTA 1
#define MAXPUBS 1
#define MAXSUBS 1

/*---------------------------------------------------------------------------*/
/*-----------------------------Declarations----------------------------------*/
/* Variables */

/* Structs */
struct topicEntry;
struct topicQueue;
struct timeval;
/* Functions */
void *publisher(void *args);
void *subscriber(void *args);
void *cleaner(void *args);
//void timersub(struct timeval *a,struct timeval *b, struct timeval *res);
int pthread_create(pthread_t *thread,const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
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
} pArgs;

typedef struct {
    int threadId;
    int lastEntry;
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
pArgs pubargs[MAXPUBS];
sArgs subargs[MAXSUBS];
pthread_t pubs[MAXPUBS];
pthread_t subs[MAXSUBS];
pthread_mutex_t mutex[MAXTOPICS];

char *topics[] = {"dogs","cats","horses", "bugs","snakes"};

int enqueue(topicEntry *entry, int q) {
    if(store[q]->buffer[(store[q]->head +1)].entryNum == -1) {
        // the queue is full, make the thread wait until there is a dequeue
        return 0;
    }
    else {
        // set the topic number 
        int c = (store[q]->counter) + 1;
        // printf("Topic Number is: %d\n",c);
        store[q]->counter++;
        entry->entryNum = c;

        // set the timestamp
        struct timeval stamp;
        gettimeofday(&stamp,NULL);
        entry->timeStamp = stamp;
        // if the entry isn't going to be the first in the buffer
        if (!((store[q]->head == store[q]->tail) && (store[q]->buffer[store[q]->head].entryNum == 0))){
            if (store[q]->head == (store[q]->length)) {
                // if head is currently at the top of the buffer, new head val = 0
                store[q]->head = 0;
            }
            else {
                store[q]->head++;
            }
        }
        store[q]->buffer[store[q]->head] = *entry;
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
            empty->entryNum = store[q]->buffer[y].entryNum;
            empty->timeStamp = store[q]->buffer[y].timeStamp;
            empty->pubID = store[q]->buffer[y].pubID;
            strcpy(empty->photoURL,store[q]->buffer[y].photoURL);
            strcpy(empty->photoCaption, store[q]->buffer[y].photoCaption);
            return 1;
        }
        else if (store[q]->buffer[y].entryNum > (lastEntry + 1)) {
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
        return 1;
    }
    return 0;
}

void *publisher(void *args) {
    int x;
    // where you are going to call enqueue with mutex locks
    while(1) {
        for (x = 0; x < MAXTOPICS; x++) {
            pthread_mutex_lock(&(mutex[x]));
            printf("\nPub Thread #%d: ",((pArgs *)args)->threadId);
            topicEntry test = {.photoCaption = "test entry"};
            int y = enqueue(&test,x);

            if (!y) {
                printf("Topic thread is full, cannot add new entry\n");
                pthread_mutex_unlock(&(mutex[x]));
                sleep(1);
                sched_yield();
            }
            else {
                printf("Added new entry to topic thread\n");
                pthread_mutex_unlock(&(mutex[x]));
            }
        }
    }
}

void *subscriber(void *args) {
    int returned = 0;
    int q;
    int last;
    // where you are going to call getEntry with mutex locks
    while(1) {
        for (q = 0; q < MAXTOPICS; q++) {
            pthread_mutex_lock(&(mutex[q]));
            topicEntry test = {};
            last = ((sArgs *) args)->lastEntry;
            // printf("\nLast accessed entry by thread %d was %d\n",((sArgs *)args)->threadId,((sArgs *) args)->lastEntry );
            returned = getEntry(last,&test,q);
            if (returned > 1) {
                printf("Sub Thread #%d: Original entry was dequeued, found next entry w caption: %s\n",((sArgs *)args)->threadId,test.photoCaption);
                ((sArgs *) args)->lastEntry = returned;
                pthread_mutex_unlock(&(mutex[q]));
            }
            else if (returned == 0) {
                printf("Sub Thread #%d: Queue is empty or entry is not queued yet\n",((sArgs *)args)->threadId);
                pthread_mutex_unlock(&(mutex[q]));
                sleep(1);
                sched_yield();
            }
            else {
                printf("Sub Thread #%d: Found next entry w/ caption: %s\n",((sArgs *)args)->threadId,test.photoCaption);
                ((sArgs *) args)->lastEntry++;
                pthread_mutex_unlock(&(mutex[q]));
            }
        }
    }
}

void *cleaner(void *args) {
    int v = 0;
    // where you are going to call dequeue with mutex locks round robin style on all the queues
    while(1) {
        for (int q = 0; q < MAXTOPICS; q++) {
            pthread_mutex_lock(&(mutex[q]));
            v = dequeue(q);

            if (v == 1) {
                printf("Dequeued from topic %d\n",q);
            }
            // else if (v == -1) {
            //     printf("\nDEQUEUE: found empty queue\n");
            // }
            // else {
            //     printf("\nDEQUEUE: found nothing old enough to dequeue\n");
            // }
            pthread_mutex_unlock(&(mutex[q]));
            sleep(1);
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

    TQ_DEF(q1,topics[0],MAXENTRIES);
    printf("Name: %s, Head: %d, Tail: %d, Length: %d\n",q1.name,q1.head,q1.tail,q1.length);
    q1.buffer[MAXENTRIES] = last;
    store[0] = &q1;
    store[0]->buffer[0].entryNum = 0;

    // MORE queues for testing. TO use, change MAXTOPICS up top
    // TQ_DEF(q2,topics[1],MAXENTRIES);
    // printf("Name: %s, Head: %d, Tail: %d, Length: %d\n",q2.name,q2.head,q2.tail,q2.length);
    // q2.buffer[MAXENTRIES] = last;
    // store[1] = &q2;
    // store[1]->buffer[0].entryNum = 0;

    // TQ_DEF(q3,topics[2],MAXENTRIES);
    // printf("Name: %s, Head: %d, Tail: %d, Length: %d\n",q3.name,q3.head,q3.tail,q3.length);
    // q3.buffer[MAXENTRIES] = last;
    // printf("Null topic entryNumber: %d\n",q3.buffer[MAXENTRIES].entryNum);
    // store[2] = &q3;
    // store[2]->buffer[0].entryNum = 0;

    printf("------------------------------Test for part1 starts---------------------------------\n");

    // initialize mutex locks for all buffers
    for (int k = 0; k < MAXTOPICS; k++) {
        pthread_mutex_init(&mutex[k],NULL);
        printf("Queue %d name: %s\n",k, store[k]->name);
    }

    // create publisher threads
    for (int i = 0; i < MAXPUBS; i++) {
        printf("Spawning publisher thread\n");
        pubargs[i].threadId = i + 1;
        c = pthread_create(&pubs[i],&attr,publisher, (void*) &pubargs[i]);
        if (c) {
            printf("Problem creating publisher thread\n");
        }
    }

    sleep(4);

    // create subscriber threads
    for (int i = 0; i < MAXSUBS; i++) {
        printf("Spawning subscriber thread\n");
        subargs[i].threadId = i + 1;
        subargs[i].lastEntry = 0;
        c = pthread_create(&subs[i],&attr,subscriber, (void*) &subargs[i]);
        if (c) {
            printf("Problem creating subscriber thread\n");
        }
    }

    // create cleaning threads
    pthread_t clean;
    c = pthread_create(&clean, &attr,cleaner, (void*) NULL );
    if (c) {
        printf("Problem creating cleaner thread\n");
    }

    sleep(4);
    printf("------------------------------Test for part1 ends---------------------------------\n");
    exit(0);
    return 1;

}
/*-----------------------------Program End-----------------------------------*/
