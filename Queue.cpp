#include "Queue.h"

Queue::Queue()
{
    front = -1;
    rear = -1;
}

bool Queue::isEmpty()
{
    return front == -1;
}

bool Queue::isFull()
{
    return rear == 49;
}

void Queue::enqueue(int value)
{
    if (isFull())
    {
        return;
    }

    if (isEmpty())
    {
        front = 0;
    }

    rear++;
    arr[rear] = value;
}

int Queue::dequeue()
{
    if (isEmpty())
    {
        return -1;
    }

    int value = arr[front];

    if (front == rear)
    {
        front = -1;
        rear = -1;
    }
    else
    {
        front++;
    }

    return value;
}
