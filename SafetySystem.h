#ifndef SAFETYSYSTEM_H
#define SAFETYSYSTEM_H

#include "RouteManager.h"
#include "DataStore.h"
#include "ConsoleUI.h"

class SafetySystem
{
private:
    ProjectData data;
    CsvDataStore dataStore;
    RouteManager routeManager;
    IncidentRiskModel incidentRiskModel;
    WeatherRiskModel weatherRiskModel;
    ConsoleUI ui;

    void initializeRouteManager();
    void displayHeader() const;
    void displayMenu(int selectedItem) const;
    void displayLocations() const;
    void displayWeather() const;
    void displaySystemInformation() const;
    void handleAddLocation();
    void handleViewMap() const;
    void handleAddDisaster();
    void handleDeactivateDisaster();
    void handleDisasterMenu();
    void handleFindSafestRoute();
    void handleFindBFSRoute();
    void handleCompareRoutes();
    void handleFindDFSRoute();
    void handleBlockRoad();
    int readInteger(const char prompt[]) const;
    void readText(const char prompt[], char buffer[], int size) const;
    int readLocationChoice(const char prompt[]) const;
    void saveData();

public:
    void initialize();
    void run();
};

#endif
