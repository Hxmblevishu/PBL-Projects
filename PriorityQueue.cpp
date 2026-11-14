#include "PriorityQueue.h"

PriorityQueue::PriorityQueue()
{
    size = 0;
}

bool PriorityQueue::isEmpty()
{
    return size == 0;
}

bool PriorityQueue::isFull()
{
    return size == 50;
}

void PriorityQueue::insert(int vertex, int cost)
{
    if (isFull())
    {
        return;
    }

    vertices[size] = vertex;
    costs[size] = cost;
    size++;
}

int PriorityQueue::extractMin(int& vertex)
{
    if (isEmpty())
    {
        vertex = -1;
        return -1;
    }

    int minIndex = 0;
    for (int i = 1; i < size; i++)
    {
        if (costs[i] < costs[minIndex])
        {
            minIndex = i;
        }
    }

    vertex = vertices[minIndex];
    int minCost = costs[minIndex];

    for (int i = minIndex; i < size - 1; i++)
    {
        vertices[i] = vertices[i + 1];
        costs[i] = costs[i + 1];
    }

    size--;
    return minCost;
}
