#include "Graph.h"
#include <iostream>
#include <iomanip>
using namespace std;

Graph::Graph()
{
    vertexCount = 0;

    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            distance[i][j] = 0;
            risk[i][j] = 0;
            blocked[i][j] = 0;
        }
    }
}

int Graph::getVertexCount()
{
    return vertexCount;
}

void Graph::setVertexCount(int count)
{
    if (count >= 0 && count <= MAX_VERTICES)
    {
        vertexCount = count;
    }
}

bool Graph::addRoad(int src, int dest, int dist, int riskLevel, bool undirected)
{
    if (src < 0 || dest < 0 || src >= MAX_VERTICES || dest >= MAX_VERTICES)
    {
        return false;
    }

    if (dist <= 0)
    {
        return false;
    }

    distance[src][dest] = dist;
    risk[src][dest] = riskLevel;
    blocked[src][dest] = 0;

    if (undirected)
    {
        distance[dest][src] = dist;
        risk[dest][src] = riskLevel;
        blocked[dest][src] = 0;
    }

    return true;
}

bool Graph::removeRoad(int src, int dest, bool undirected)
{
    if (src < 0 || dest < 0 || src >= MAX_VERTICES || dest >= MAX_VERTICES)
    {
        return false;
    }

    distance[src][dest] = 0;
    risk[src][dest] = 0;
    blocked[src][dest] = 0;

    if (undirected)
    {
        distance[dest][src] = 0;
        risk[dest][src] = 0;
        blocked[dest][src] = 0;
    }

    return true;
}

bool Graph::blockRoad(int src, int dest, bool undirected)
{
    if (!hasEdge(src, dest))
    {
        return false;
    }

    blocked[src][dest] = 1;

    if (undirected)
    {
        blocked[dest][src] = 1;
    }

    return true;
}

bool Graph::unblockRoad(int src, int dest, bool undirected)
{
    if (!hasEdge(src, dest))
    {
        return false;
    }

    blocked[src][dest] = 0;

    if (undirected)
    {
        blocked[dest][src] = 0;
    }

    return true;
}

bool Graph::hasEdge(int src, int dest)
{
    if (src < 0 || dest < 0 || src >= MAX_VERTICES || dest >= MAX_VERTICES)
    {
        return false;
    }

    return distance[src][dest] > 0;
}

bool Graph::isRoadBlocked(int src, int dest)
{
    if (!hasEdge(src, dest))
    {
        return false;
    }

    return blocked[src][dest] == 1;
}

int Graph::getDistance(int src, int dest)
{
    if (!hasEdge(src, dest))
    {
        return 0;
    }

    return distance[src][dest];
}

int Graph::getRisk(int src, int dest)
{
    if (!hasEdge(src, dest))
    {
        return 0;
    }

    return risk[src][dest];
}

void Graph::displayMap(Location locations[], int locationCount)
{
    cout << "\n========== ROAD MAP ==========\n";

    if (locationCount == 0)
    {
        cout << "No locations available.\n";
        return;
    }

    cout << left << setw(14) << "From/To";
    for (int i = 0; i < locationCount; i++)
    {
        cout << std::setw(10) << locations[i].getName();
    }
    cout << "\n";

    for (int i = 0; i < locationCount; i++)
    {
        cout << setw(14) << locations[i].getName();
        for (int j = 0; j < locationCount; j++)
        {
            if (i == j)
            {
                cout << setw(10) << "-";
            }
            else if (hasEdge(i, j))
            {
                if (blocked[i][j] == 1)
                {
                    cout << setw(10) << "BLOCKED";
                }
                else
                {
                    cout << setw(10) << distance[i][j];
                }
            }
            else
            {
                cout << setw(10) << "0";
            }
        }
        cout << "\n";
    }

    cout << "\nRoad Details:\n";
    for (int i = 0; i < locationCount; i++)
    {
        for (int j = 0; j < locationCount; j++)
        {
            if (i != j && hasEdge(i, j) && i < j)
            {
                cout << locations[i].getName() << " -> " << locations[j].getName()
                          << " | Distance: " << distance[i][j]
                          << " km | Risk: " << risk[i][j];

                if (blocked[i][j] == 1)
                {
                    cout << " | Status: BLOCKED";
                }

                cout << "\n";
            }
        }
    }

    cout << "==============================\n";
}
