#ifndef QUEUE_H
#define QUEUE_H

class Queue
{
private:
    int arr[50];
    int front;
    int rear;

public:
    Queue();

    bool isEmpty();
    bool isFull();

    void enqueue(int value);
    int dequeue();
};

#endif
