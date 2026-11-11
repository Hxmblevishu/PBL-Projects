#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include "Graph.h"
#include "Disaster.h"
#include "Location.h"
#include "RiskModel.h"

const int RISK_WEIGHT = 5;

struct RouteResult
{
    int path[50];
    int pathLength;
    int totalDistance;
    int totalSafetyCost;
    int totalRisk;
    bool found;
};

class RouteManager
{
private:
    Graph* graph;
    DisasterManager* disasterManager;
    Location* locations;
    WeatherSnapshot* weather;
    RiskModel* incidentRiskModel;
    RiskModel* weatherRiskModel;
    int locationCount;

    int calculateSafetyCost(int distance, int risk);
    bool isRoadUsable(int src, int dest);
    int getEffectiveRisk(int src, int dest);
    void reconstructPath(int parent[], int source, int destination, RouteResult& result);
    const char* getRiskLevel(int totalRisk);

public:
    RouteManager();

    void initialize(Graph* graph, DisasterManager* disasterManager, Location locations[],
                    WeatherSnapshot weather[], int locationCount,
                    RiskModel* incidentRiskModel, RiskModel* weatherRiskModel);

    RouteResult findBFSRoute(int source, int destination);
    RouteResult findDFSRoute(int source, int destination);
    RouteResult findSafestRoute(int source, int destination);

    void displayRoute(const char* title, RouteResult result);
};

#endif
