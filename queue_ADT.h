#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

typedef struct node
{
    void* data;
    struct node *next;
}NODE;

typedef struct queue
{
    NODE *front;
    NODE *rear;
    int size;
}QUEUE;

void init(QUEUE *t)
{
    t->front=NULL;
    t->rear=NULL;
    t->size=0;
}

NODE *createNode(void *dataIn)
{
    NODE *pNew=(NODE *)malloc(sizeof(NODE));
    pNew->data=dataIn;
    pNew->next=NULL;
    return pNew;
}
void enqueue(QUEUE *t,void *dataIn)
{
    NODE *pNew=createNode(dataIn);
    if(t->front==NULL)
        t->front=pNew;
    else
        (t->rear)->next=pNew;
    t->rear=pNew;
    t->size++;
}

void *dequeue(QUEUE *t)
{
    NODE *pDlt;
    void *dataOut;
	if(t->front==NULL)
        printf("\nError : UNDERFLOW");
	else
	{
		pDlt=t->front;
		dataOut=pDlt->data;
		t->front=pDlt->next;
		free(pDlt);
		t->size--;
	}
    return dataOut;
}

int isEmpty(QUEUE t)
{
    return t.front==NULL;
}
