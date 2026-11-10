#ifndef GRAPH_H
#define GRAPH_H

#include "Location.h"

const int MAX_VERTICES = 50;

class Graph
{
private:
    int distance[MAX_VERTICES][MAX_VERTICES];
    int risk[MAX_VERTICES][MAX_VERTICES];
    int blocked[MAX_VERTICES][MAX_VERTICES];
    int vertexCount;

public:
    Graph();

    int getVertexCount();
    void setVertexCount(int count);

    bool addRoad(int src, int dest, int dist, int riskLevel, bool undirected);
    bool removeRoad(int src, int dest, bool undirected);
    bool blockRoad(int src, int dest, bool undirected);
    bool unblockRoad(int src, int dest, bool undirected);

    bool hasEdge(int src, int dest);
    bool isRoadBlocked(int src, int dest);
    int getDistance(int src, int dest);
    int getRisk(int src, int dest);

    void displayMap(Location locations[], int locationCount);
};

#endif
