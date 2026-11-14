#include "RouteManager.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "ConsoleUI.h"
#include <iostream>
using namespace std;

RouteManager::RouteManager()
{
    graph = 0;
    disasterManager = 0;
    locations = 0;
    weather = 0;
    incidentRiskModel = 0;
    weatherRiskModel = 0;
    locationCount = 0;
}

void RouteManager::initialize(Graph* graph, DisasterManager* disasterManager, Location locations[],
                              WeatherSnapshot weather[], int locationCount,
                              RiskModel* incidentRiskModel, RiskModel* weatherRiskModel)
{
    this->graph = graph;
    this->disasterManager = disasterManager;
    this->locations = locations;
    this->weather = weather;
    this->locationCount = locationCount;
    this->incidentRiskModel = incidentRiskModel;
    this->weatherRiskModel = weatherRiskModel;
}

int RouteManager::calculateSafetyCost(int distance, int risk)
{
    return distance + (risk * RISK_WEIGHT);
}

bool RouteManager::isRoadUsable(int src, int dest)
{
    if (!graph->hasEdge(src, dest))
    {
        return false;
    }

    if (graph->isRoadBlocked(src, dest))
    {
        return false;
    }

    if (disasterManager->isRoadBlockedByDisaster(src, dest))
    {
        return false;
    }

    return true;
}

int RouteManager::getEffectiveRisk(int src, int dest)
{
    int baseRisk = graph->getRisk(src, dest);
    baseRisk = incidentRiskModel->assess(baseRisk, src, dest, weather, *disasterManager);
    return weatherRiskModel->assess(baseRisk, src, dest, weather, *disasterManager);
}

void RouteManager::reconstructPath(int parent[], int source, int destination, RouteResult& result)
{
    int tempPath[50];
    int length = 0;
    int current = destination;

    while (current != -1)
    {
        tempPath[length] = current;
        length++;

        if (current == source)
        {
            break;
        }

        current = parent[current];
    }

    if (length == 0 || tempPath[length - 1] != source)
    {
        result.found = false;
        result.pathLength = 0;
        return;
    }

    result.pathLength = length;
    for (int i = 0; i < length; i++)
    {
        result.path[i] = tempPath[length - 1 - i];
    }
}

const char* RouteManager::getRiskLevel(int totalRisk)
{
    if (totalRisk <= 3)
    {
        return "LOW";
    }
    if (totalRisk <= 8)
    {
        return "MEDIUM";
    }
    return "HIGH";
}

RouteResult RouteManager::findBFSRoute(int source, int destination)
{
    RouteResult result;
    result.found = false;
    result.pathLength = 0;
    result.totalDistance = 0;
    result.totalSafetyCost = 0;
    result.totalRisk = 0;

    if (source == destination)
    {
        result.found = true;
        result.path[0] = source;
        result.pathLength = 1;
        return result;
    }

    if (source < 0 || destination < 0 || source >= locationCount || destination >= locationCount)
    {
        return result;
    }

    bool visited[50];
    int parent[50];

    for (int i = 0; i < 50; i++)
    {
        visited[i] = false;
        parent[i] = -1;
    }

    Queue queue;
    queue.enqueue(source);
    visited[source] = true;

    while (!queue.isEmpty())
    {
        int current = queue.dequeue();

        if (current == destination)
        {
            break;
        }

        for (int neighbour = 0; neighbour < locationCount; neighbour++)
        {
            if (!visited[neighbour] && isRoadUsable(current, neighbour))
            {
                visited[neighbour] = true;
                parent[neighbour] = current;
                queue.enqueue(neighbour);
            }
        }
    }

    if (!visited[destination])
    {
        return result;
    }

    reconstructPath(parent, source, destination, result);
    result.found = true;

    for (int i = 0; i < result.pathLength - 1; i++)
    {
        int from = result.path[i];
        int to = result.path[i + 1];
        result.totalDistance += graph->getDistance(from, to);
        result.totalRisk += getEffectiveRisk(from, to);
    }

    result.totalSafetyCost = calculateSafetyCost(result.totalDistance, result.totalRisk);
    return result;
}

RouteResult RouteManager::findDFSRoute(int source, int destination)
{
    RouteResult result;
    result.found = false;
    result.pathLength = 0;
    result.totalDistance = 0;
    result.totalSafetyCost = 0;
    result.totalRisk = 0;

    if (source < 0 || destination < 0 || source >= locationCount || destination >= locationCount)
    {
        return result;
    }

    bool visited[50];
    int parent[50];
    int stack[50];
    for (int i = 0; i < 50; i++)
    {
        visited[i] = false;
        parent[i] = -1;
    }

    int top = -1;
    stack[++top] = source;
    visited[source] = true;

    while (top >= 0)
    {
        int current = stack[top--];
        if (current == destination) break;

        // Push in reverse order so the smallest place ID is explored first.
        for (int neighbour = locationCount - 1; neighbour >= 0; neighbour--)
        {
            if (!visited[neighbour] && isRoadUsable(current, neighbour))
            {
                visited[neighbour] = true;
                parent[neighbour] = current;
                stack[++top] = neighbour;
            }
        }
    }

    if (!visited[destination]) return result;

    reconstructPath(parent, source, destination, result);
    result.found = true;
    for (int i = 0; i < result.pathLength - 1; i++)
    {
        int from = result.path[i];
        int to = result.path[i + 1];
        result.totalDistance += graph->getDistance(from, to);
        result.totalRisk += getEffectiveRisk(from, to);
    }
    result.totalSafetyCost = calculateSafetyCost(result.totalDistance, result.totalRisk);
    return result;
}

RouteResult RouteManager::findSafestRoute(int source, int destination)
{
    RouteResult result;
    result.found = false;
    result.pathLength = 0;
    result.totalDistance = 0;
    result.totalSafetyCost = 0;
    result.totalRisk = 0;

    if (source == destination)
    {
        result.found = true;
        result.path[0] = source;
        result.pathLength = 1;
        return result;
    }

    if (source < 0 || destination < 0 || source >= locationCount || destination >= locationCount)
    {
        return result;
    }

    int safetyCost[50];
    int parent[50];
    bool visited[50];

    for (int i = 0; i < 50; i++)
    {
        safetyCost[i] = 999999;
        parent[i] = -1;
        visited[i] = false;
    }

    safetyCost[source] = 0;
    PriorityQueue pq;
    pq.insert(source, 0);

    while (!pq.isEmpty())
    {
        int current;
        pq.extractMin(current);

        if (visited[current])
        {
            continue;
        }

        visited[current] = true;

        if (current == destination)
        {
            break;
        }

        for (int neighbour = 0; neighbour < locationCount; neighbour++)
        {
            if (!isRoadUsable(current, neighbour))
            {
                continue;
            }

            int edgeDistance = graph->getDistance(current, neighbour);
            int edgeRisk = getEffectiveRisk(current, neighbour);
            int edgeCost = calculateSafetyCost(edgeDistance, edgeRisk);
            int newCost = safetyCost[current] + edgeCost;

            if (newCost < safetyCost[neighbour])
            {
                safetyCost[neighbour] = newCost;
                parent[neighbour] = current;
                pq.insert(neighbour, newCost);
            }
        }
    }

    if (safetyCost[destination] == 999999)
    {
        return result;
    }

    reconstructPath(parent, source, destination, result);
    result.found = true;
    result.totalSafetyCost = safetyCost[destination];

    for (int i = 0; i < result.pathLength - 1; i++)
    {
        int from = result.path[i];
        int to = result.path[i + 1];
        result.totalDistance += graph->getDistance(from, to);
        result.totalRisk += getEffectiveRisk(from, to);
    }

    return result;
}

void RouteManager::displayRoute(const char* title, RouteResult result)
{
    ConsoleUI ui;
    ui.setColor(ConsoleUI::PINK);
    cout << "\n  " << title << "\n";
    ui.divider('=');
    ui.resetColor();

    if (!result.found)
    {
        ui.setColor(ConsoleUI::RED);
        cout << "  No available route connects those places.\n";
        ui.resetColor();
        ui.divider();
        return;
    }

    if (result.pathLength == 1)
    {
        cout << "  Start and destination are the same place: ";
        ui.setColor(ConsoleUI::LIGHT_BLUE);
        cout << locations[result.path[0]].getName() << "\n";
        ui.resetColor();
        ui.divider();
        return;
    }

    cout << "  ";
    for (int i = 0; i < result.pathLength; i++)
    {
        ui.setColor(ConsoleUI::LIGHT_BLUE);
        cout << locations[result.path[i]].getName();
        ui.resetColor();
        if (i < result.pathLength - 1)
            cout << "  ->  ";
    }

    cout << "\n";
    ui.setColor(ConsoleUI::PINK); << cout"  DISTANCE       ";
    ui.setColor(ConsoleUI::WHITE); cout << result.totalDistance << " km\n";
    ui.setColor(ConsoleUI::PINK); cout << "  SAFETY COST    ";
    ui.setColor(ConsoleUI::WHITE); cout << result.totalSafetyCost << "\n";
    ui.setColor(result.totalRisk > 8 ? ConsoleUI::RED :
                result.totalRisk > 3 ? ConsoleUI::PINK : ConsoleUI::LIGHT_BLUE);
    cout << "  RISK LEVEL     " << getRiskLevel(result.totalRisk) << "\n";
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << "  ROUTE STATUS   AVAILABLE\n";
    ui.resetColor();
    ui.divider();
}
