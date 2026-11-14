#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

class PriorityQueue
{
private:
    int vertices[50];
    int costs[50];
    int size;

public:
    PriorityQueue();

    bool isEmpty();
    bool isFull();

    void insert(int vertex, int cost);
    int extractMin(int& vertex);
};

#endif
