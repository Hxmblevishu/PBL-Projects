#include "SafetySystem.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;
namespace
{
string fitPanelText(const string& text, int width)
{
    return static_cast<int>(text.length()) > width ? text.substr(0, width) : text;
}

void drawDashboardBorder(const ConsoleUI& ui)
{
    ui.setColor(ConsoleUI::BLUE);
    cout << "  +" << string(44, '-') << "+--+"
              << string(38, '-') << "+\n";
    ui.resetColor();
}

void drawDashboardRow(const ConsoleUI& ui, const std::string& left, const std::string& right,
                      ConsoleUI::Color leftColor, ConsoleUI::Color rightColor, bool selected = false)
{
    cout << "  |";
    if (selected)
    {
        ui.setColor(ConsoleUI::PINK);
        cout << " >";
        ui.setColor(ConsoleUI::SELECTED);
        cout << left << setw(42) << fitPanelText(left, 42);
    }
    else
    {
        cout << "  ";
        ui.setColor(leftColor);
        cout << left << setw(42) << fitPanelText(left, 42);
    }
    ui.resetColor();
    cout << "|--|";
    ui.setColor(rightColor);
    cout << left << setw(38) << fitPanelText(right, 38);
    ui.resetColor();
    cout << "|\n";
}
}

void SafetySystem::initializeRouteManager()
{
    routeManager.initialize(&data.graph, &data.disasters, data.locations, data.weather,
                            data.locationCount, &incidentRiskModel, &weatherRiskModel);
}

void SafetySystem::initialize()
{
    ui.initialize();
    if (!dataStore.load(data, "data"))
    {
        ui.setColor(ConsoleUI::RED);
        cout << "Could not load data/locations.csv. Check the project data folder.\n";
        ui.resetColor();
        return;
    }
    initializeRouteManager();
}

int SafetySystem::readInteger(const char prompt[]) const
{
    string line;
    int value;
    while (true)
    {
        ui.setColor(ConsoleUI::LIGHT_BLUE);
        cout << prompt;
        ui.resetColor();
        if (!getline(cin, line)) return -1;
        stringstream input(line);
        char extra;
        if ((input >> value) && !(input >> extra)) return value;
        ui.setColor(ConsoleUI::PINK);
        cout << "Enter a whole number.\n";
        ui.resetColor();
    }
}

void SafetySystem::readText(const char prompt[], char buffer[], int size) const
{
    std::string value;
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << prompt;
    ui.resetColor();
    getline(cin, value);
    if (size <= 0) return;
    int i = 0;
    while (i < size - 1 && i < static_cast<int>(value.length()))
    {
        buffer[i] = value[i];
        i++;
    }
    buffer[i] = '\0';
}

int SafetySystem::readLocationChoice(const char prompt[]) const
{
    if (data.locationCount == 0)
    {
        ui.setColor(ConsoleUI::PINK);
        cout << "No locations are loaded.\n";
        ui.resetColor();
        return -1;
    }
    for (int i = 0; i < data.locationCount; i++)
        cout << "  " << setw(2) << data.locations[i].getId() << "  "
                  << data.locations[i].getName() << "\n";
    int id = readInteger(prompt);
    if (id < 0 || id >= data.locationCount)
    {
        ui.setColor(ConsoleUI::RED);
        cout << "That location ID is not in the list.\n";
        ui.resetColor();
        return -1;
    }
    return id;
}

void SafetySystem::saveData()
{
    if (!dataStore.save(data, "data"))
    {
        ui.setColor(ConsoleUI::RED);
        cout << "Could not save the project data. Check write access to the data folder.\n";
        ui.resetColor();
    }
}

void SafetySystem::displayHeader() const
{
    int roadCount = 0;
    int weatherCount = 0;
    int blockedCount = 0;
    for (int i = 0; i < data.locationCount; i++)
    {
        if (data.weather[i].available) weatherCount++;
        for (int j = i + 1; j < data.locationCount; j++)
        {
            if (data.graph.hasEdge(i, j))
            {
                roadCount++;
                if (data.graph.isRoadBlocked(i, j)) blockedCount++;
            }
        }
    }

    ui.printBanner();
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << "  PLACES ";
    ui.setColor(ConsoleUI::WHITE);
    cout << data.locationCount << "   ";
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << "ROAD LINKS ";
    ui.setColor(ConsoleUI::WHITE);
    cout << roadCount << "   ";
    ui.setColor(ConsoleUI::RED);
    cout << "BLOCKED ";
    ui.setColor(ConsoleUI::WHITE);
    cout << blockedCount << "   ";
    ui.setColor(ConsoleUI::PINK);
    cout << "WEATHER SNAPSHOT ";
    ui.setColor(ConsoleUI::WHITE);
    cout << weatherCount << "/" << data.locationCount;
    ui.resetColor();
    cout << "\n";
}

void SafetySystem::displayMenu(int selectedItem) const
{
    static const char* menuItems[] = {
        "01  BFS PATH - FEWEST HOPS",
        "02  DFS PATH - DEPTH FIRST",
        "03  DIJKSTRA SAFEST PATH",
        "04  ADD LOCATION",
        "05  SHOW ROAD MAP",
        "06  ADD SITUATION RISK",
        "07  BLOCK / UNBLOCK ROAD",
        "08  COMPARE BFS + DFS + DIJKSTRA",
        "09  DISASTER INFORMATION",
        "10  SYSTEM INFORMATION",
        "11  EXIT SURAKSHAPATH"
    };
    static const char* guide[] = {
        "Fewest road links between places",
        "Depth-first path; may use more links",
        "Lowest distance + road risk cost",
        "Save a new Dehradun place",
        "See the loaded road network",
        "Report an event and severity",
        "Change a road's open status",
        "Compare all three route methods",
        "View or manage incident reports",
        "Places, data, and algorithms",
        "Close the route desk"
    };

    cout << "\n";
    drawDashboardBorder(ui);
    drawDashboardRow(ui, "ROUTE OPTIONS", "QUICK GUIDE", ConsoleUI::PINK, ConsoleUI::LIGHT_BLUE);
    drawDashboardBorder(ui);
    for (int i = 0; i < 11; i++)
    {
        ConsoleUI::Color guideColor = (i == 5 || i == 6 || i == 8) ? ConsoleUI::RED : ConsoleUI::WHITE;
        drawDashboardRow(ui, menuItems[i], guide[i], ConsoleUI::WHITE, guideColor, i == selectedItem);
    }
    drawDashboardBorder(ui);
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << "  [UP]/[DOWN] MOVE";
    ui.setColor(ConsoleUI::PINK);
    cout << "     [ENTER] SELECT";
    ui.setColor(ConsoleUI::WHITE);
    cout << "     [1-11] + ENTER";
    ui.setColor(ConsoleUI::RED);
    cout << "     [ESC] EXIT\n";
    ui.resetColor();
}

void SafetySystem::displayLocations() const
{
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << "\n  PROMINENT PLACES\n";
    ui.resetColor();
    cout << "  " << left << setw(5) << "ID" << setw(49) << "Place" << "Weather\n";
    ui.setColor(ConsoleUI::BLUE);
    cout << "  ------------------------------------------------------------------------\n";
    ui.resetColor();
    for (int i = 0; i < data.locationCount; i++)
    {
        cout << "  " << left << setw(5) << data.locations[i].getId()
                  << setw(49) << data.locations[i].getName()
                  << (data.weather[i].available ? "synced" : "pending") << "\n";
    }
}

void SafetySystem::displayWeather() const
{
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << "\n  SAVED WEATHER / ROUTE RISK INPUT\n";
    ui.resetColor();
    cout << "  Risk scale: 1-5 per road. Rain >= 5/15/30 mm adds 1/2/3; wind >= 40/60/80 km/h adds 1/2/3; combined weather uplift is capped at 4.\n";
    cout << "  " << left << setw(31) << "Place" << setw(22) << "Observed (local)"
              << setw(10) << "Temp C" << setw(12) << "Rain mm" << "Wind km/h\n";
    ui.setColor(ConsoleUI::BLUE);
    cout << "  -------------------------------------------------------------------------------\n";
    ui.resetColor();
    for (int i = 0; i < data.locationCount; i++)
    {
        const WeatherSnapshot& snapshot = data.weather[i];
        cout << "  " << left << setw(31) << data.locations[i].getName();
        if (!snapshot.available)
        {
            ui.setColor(ConsoleUI::PINK);
            cout << "not synced\n";
            ui.resetColor();
        }
        else
            cout << setw(22) << snapshot.observedAt << setw(10) << snapshot.temperatureC
                      << setw(12) << snapshot.precipitationMm << snapshot.windKmh << "\n";
    }
}

void SafetySystem::displaySystemInformation() const
{
    ui.setColor(ConsoleUI::PINK);
    cout << "\n  SYSTEM INFORMATION\n";
    ui.divider('=');
    ui.setColor(ConsoleUI::WHITE);
    cout << "  Area: Dehradun local road network\n";
    cout << "  Storage: locations, roads, weather, and incidents are saved in data/*.csv\n";
    cout << "  BFS: finds a route with the fewest road links.\n";
    cout << "  DFS: follows a depth-first path; it may use more links than BFS.\n";
    cout << "  Dijkstra: minimizes distance plus the road and situation risk cost.\n";
    cout << "  Weather: uses the saved snapshot; it does not refresh while the app runs.\n";
    ui.resetColor();
    displayLocations();
    displayWeather();
}

void SafetySystem::handleAddLocation()
{
    if (data.locationCount >= MAX_LOCATIONS)
    {
        ui.setColor(ConsoleUI::RED);
        cout << "The location limit of " << MAX_LOCATIONS << " has been reached.\n";
        ui.resetColor();
        return;
    }
    char name[64];
    readText("Place name: ", name, sizeof(name));
    if (name[0] == '\0') return;

    int id = data.locationCount;
    data.locations[id].setLocation(id, name);
    data.locationCount++;

    bool connected = false;
    if (readInteger("Connect it to the road network now? 1 = yes, 0 = later: ") == 1)
    {
        int other = readLocationChoice("Connect the new place to an existing place ID: ");
        if (other >= 0 && other != id)
        {
            int distance = readInteger("Road distance (km): ");
            int risk = readInteger("Base road risk (1-5): ");
            connected = data.graph.addRoad(id, other, distance, risk, true);
        }
        else if (other == id)
        {
            ui.setColor(ConsoleUI::PINK);
            cout << "Choose a different place to connect to.\n";
            ui.resetColor();
        }
    }

    initializeRouteManager();
    saveData();
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << (connected ? "Place and road link saved.\n" :
                              "Place saved. It needs a road link before it can appear in routes.\n");
    ui.resetColor();
}

void SafetySystem::handleViewMap() const
{
    data.graph.displayMap(data.locations, data.locationCount);
}

void SafetySystem::handleAddDisaster()
{
    ui.setColor(ConsoleUI::PINK);
    cout << "\n  ADD SITUATION RISK\n";
    ui.divider();
    ui.resetColor();
    int location = readLocationChoice("Incident location ID: ");
    if (location < 0) return;
    char type[64];
    readText("Incident type: ", type, sizeof(type));
    int severity = readInteger("Severity (1-5): ");
    if (data.disasters.addDisaster(location, type, severity))
    {
        saveData();
        ui.setColor(ConsoleUI::LIGHT_BLUE);
        cout << "Incident saved. Severity 5 closes roads touching this place.\n";
        ui.resetColor();
    }
    else
    {
        ui.setColor(ConsoleUI::RED);
        cout << "Could not record it. Severity must be 1-5.\n";
        ui.resetColor();
    }
}

void SafetySystem::handleDeactivateDisaster()
{
    int id = readInteger("Incident ID to close: ");
    bool found = false;
    for (int i = 0; i < data.disasters.getDisasterCount(); i++)
        if (data.disasters.getDisasterAt(i).getDisasterId() == id) found = true;
    if (!found)
    {
        ui.setColor(ConsoleUI::PINK);
        cout << "No incident has that ID.\n";
        ui.resetColor();
        return;
    }
    data.disasters.deactivateDisaster(id);
    saveData();
    ui.setColor(ConsoleUI::LIGHT_BLUE);
    cout << "Incident closed and saved.\n";
    ui.resetColor();
}

void SafetySystem::handleDisasterMenu()
{
    ui.setColor(ConsoleUI::PINK);
    cout << "\n  DISASTER INFORMATION\n";
    ui.divider();
    ui.setColor(ConsoleUI::WHITE);
    cout << "  1  Add situation risk\n  2  View active and saved incidents\n  3  Close an incident\n  4  Back\n";
    int choice = readInteger("Choose 1-4: ");
    switch (choice)
    {
    case 1: handleAddDisaster(); break;
    case 2: data.disasters.viewDisasters(); break;
    case 3: handleDeactivateDisaster(); break;
    default: break;
    }
}

void SafetySystem::handleFindSafestRoute()
{
    int source = readLocationChoice("Start ID: ");
    if (source < 0) return;
    int destination = readLocationChoice("Destination ID: ");
    if (destination < 0) return;
    routeManager.displayRoute("DIJKSTRA SAFEST ROUTE", routeManager.findSafestRoute(source, destination));
}

void SafetySystem::handleFindBFSRoute()
{
    int source = readLocationChoice("Start ID: ");
    if (source < 0) return;
    int destination = readLocationChoice("Destination ID: ");
    if (destination < 0) return;
    routeManager.displayRoute("BFS PATH - FEWEST HOPS", routeManager.findBFSRoute(source, destination));
}

void SafetySystem::handleCompareRoutes()
{
    int source = readLocationChoice("Start ID: ");
    if (source < 0) return;
    int destination = readLocationChoice("Destination ID: ");
    if (destination < 0) return;

    routeManager.displayRoute("BFS PATH - FEWEST HOPS", routeManager.findBFSRoute(source, destination));
    routeManager.displayRoute("DFS PATH - DEPTH FIRST", routeManager.findDFSRoute(source, destination));
    routeManager.displayRoute("DIJKSTRA SAFEST ROUTE", routeManager.findSafestRoute(source, destination));
}

void SafetySystem::handleFindDFSRoute()
{
    int source = readLocationChoice("Start ID: ");
    if (source < 0) return;
    int destination = readLocationChoice("Destination ID: ");
    if (destination < 0) return;
    routeManager.displayRoute("DFS PATH - DEPTH FIRST", routeManager.findDFSRoute(source, destination));
}

void SafetySystem::handleBlockRoad()
{
    int source = readLocationChoice("Road starts at ID: ");
    if (source < 0) return;
    int destination = readLocationChoice("Road ends at ID: ");
    if (destination < 0) return;
    int action = readInteger("1 = block, 2 = unblock: ");
    bool changed = action == 1 ? data.graph.blockRoad(source, destination, true) :
                   action == 2 ? data.graph.unblockRoad(source, destination, true) : false;
    if (changed)
    {
        saveData();
        ui.setColor(ConsoleUI::LIGHT_BLUE);
        cout << (action == 1 ? "Road blocked and saved.\n" : "Road unblocked and saved.\n");
        ui.resetColor();
    }
    else
    {
        ui.setColor(ConsoleUI::PINK);
        cout << "No change made. Check the road IDs and action.\n";
        ui.resetColor();
    }
}

void SafetySystem::run()
{
    if (data.locationCount == 0) 
    {
        ui.setColor(ConsoleUI::PINK);
        cout << "No locations available. Please add locations first.\n";
        ui.resetColor();
        return;
    }
    bool running = true;
    int selectedItem = 0;
    const int menuItemCount = 11;
    while (running)
    {
        ui.clear();
        displayHeader();
        displayMenu(selectedItem);

        int key = ui.readMenuKey(menuItemCount);
        int choice = 0;
        if (key == -1)
        {
            selectedItem = (selectedItem + menuItemCount - 1) % menuItemCount;
            continue;
        }
        else if (key == 1)
        {
            selectedItem = (selectedItem + 1) % menuItemCount;
            continue;
        }
        else if (key == 0)
            choice = selectedItem + 1;
        else if (key >= 101 && key <= 100 + menuItemCount)
        {
            choice = key - 100;
            selectedItem = choice - 1;
        }
        else if (key == 2)
            break;
        else
        {
            ui.setColor(ConsoleUI::PINK);
            cout << "Choose a menu item from 1 to " << menuItemCount << ".\n";
            ui.resetColor();
            ui.pause();
            continue;
        }

        switch (choice)
        {
        case 1: 
        handleFindBFSRoute(); 
        break;
        case 2: 
        handleFindDFSRoute(); 
        break;
        case 3: 
        handleFindSafestRoute();
        break;
        case 4: 
        handleAddLocation(); 
        break;
        case 5: 
        handleViewMap(); 
        break;
        case 6: 
        handleAddDisaster(); 
        break;
        case 7: 
        handleBlockRoad(); 
        break;
        case 8: 
        handleCompareRoutes(); 
        break;
        case 9: 
        handleDisasterMenu(); 
        break;
        case 10: 
        displaySystemInformation(); 
        break;
        case 11:
            running = false;
            ui.setColor(ConsoleUI::PINK);
            cout << "\n  SurakshaPath is closed. Travel safely.\n";
            ui.resetColor();
            break;
        default:
            break;
        }
        if (running) ui.pause();
    }
}
