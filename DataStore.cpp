#include "DataStore.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

ProjectData::ProjectData()
{
    locationCount = 0;
}

static string makePath(const char directory[], const char fileName[])
{
    string path(directory);
    if (!path.empty() && path[path.length() - 1] != '/' && path[path.length() - 1] != '\\')
        path += '/';
    path += fileName;
    return path;
}

static bool readLocations(ProjectData& data, const string& path)
{
    ifstream file(path.c_str());
    if (!file.is_open()) return false;

    string line;
    getline(file, line);
    while (getline(file, line) && data.locationCount < MAX_LOCATIONS)
    {
        stringstream row(line);
        string idText, name;
        if (!getline(row, idText, ',') || !getline(row, name, ','))
            continue;

        int id = atoi(idText.c_str());
        if (id < 0 || id >= MAX_LOCATIONS || name.empty()) continue;

        data.locations[id].setLocation(id, name.c_str());
        if (id >= data.locationCount) data.locationCount = id + 1;
    }
    return data.locationCount > 0;
}

static void readRoads(ProjectData& data, const string& path)
{
    ifstream file(path.c_str());
    if (!file.is_open()) return;
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        stringstream row(line);
        string fromText, toText, distanceText, riskText, blockedText;
        if (!getline(row, fromText, ',') || !getline(row, toText, ',') ||
            !getline(row, distanceText, ',') || !getline(row, riskText, ',') ||
            !getline(row, blockedText, ','))
            continue;

        int from = atoi(fromText.c_str());
        int to = atoi(toText.c_str());
        int distance = atoi(distanceText.c_str());
        int risk = atoi(riskText.c_str());
        int blocked = atoi(blockedText.c_str());

        if (from >= 0 && to >= 0 && from < data.locationCount && to < data.locationCount &&
            from != to && distance > 0 && risk >= 1 && risk <= 5)
            data.graph.addRoad(from, to, distance, risk, true);

        if (blocked && from >= 0 && to >= 0 && from < data.locationCount && to < data.locationCount)
            data.graph.blockRoad(from, to, true);
    }
}

static void readWeather(ProjectData& data, const string& path)
{
    ifstream file(path.c_str());
    if (!file.is_open()) return;

    string line;
    getline(file, line);
    while (getline(file, line))
    {
        stringstream row(line);
        string idText, observedAt, temperatureText, precipitationText, windText, codeText;

        if (!getline(row, idText, ',') || !getline(row, observedAt, ',') ||
            !getline(row, temperatureText, ',') || !getline(row, precipitationText, ',') ||
            !getline(row, windText, ',') || !getline(row, codeText, ','))
            continue;

        int id = atoi(idText.c_str());
        if (id < 0 || id >= data.locationCount) continue;

        WeatherSnapshot& snapshot = data.weather[id];
        snapshot.available = true;

        strncpy(snapshot.observedAt, observedAt.c_str(), sizeof(snapshot.observedAt) - 1);
        snapshot.observedAt[sizeof(snapshot.observedAt) - 1] = '\0';

        snapshot.temperatureC = atof(temperatureText.c_str());
        snapshot.precipitationMm = atof(precipitationText.c_str());
        snapshot.windKmh = atof(windText.c_str());
        snapshot.weatherCode = atoi(codeText.c_str());
    }
}

static void readDisasters(ProjectData& data, const string& path)
{
    ifstream file(path.c_str());
    if (!file.is_open()) return;

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        stringstream row(line);
        string idText, locationText, type, severityText, activeText;

        if (!getline(row, idText, ',') || !getline(row, locationText, ',') ||
            !getline(row, type, ',') || !getline(row, severityText, ',') ||
            !getline(row, activeText, ','))
            continue;

        int location = atoi(locationText.c_str());

        if (location >= 0 && location < data.locationCount)
            data.disasters.restoreDisaster(atoi(idText.c_str()), location, type.c_str(),
                                           atoi(severityText.c_str()), atoi(activeText.c_str()));
    }
}

bool CsvDataStore::load(ProjectData& data, const char directory[])
{
    data = ProjectData();

    if (!readLocations(data, makePath(directory, "locations.csv"))) return false;

    readRoads(data, makePath(directory, "roads.csv"));
    readWeather(data, makePath(directory, "weather.csv"));
    readDisasters(data, makePath(directory, "disasters.csv"));

    return true;
}

bool CsvDataStore::save(const ProjectData& data, const char directory[])
{
    bool success = true;

    ofstream locationsFile(makePath(directory, "locations.csv").c_str());

    if (!locationsFile.is_open()) success = false;
    else
    {
        locationsFile << "id,name\n";

        for (int i = 0; i < data.locationCount; i++)
            locationsFile << data.locations[i].getId() << ',' << data.locations[i].getName() << '\n';
    }

    ofstream roadsFile(makePath(directory, "roads.csv").c_str());

    if (!roadsFile.is_open()) success = false;
    else
    {
        roadsFile << "from,to,distance_km,risk,blocked\n";

        for (int i = 0; i < data.locationCount; i++)
            for (int j = i + 1; j < data.locationCount; j++)
                if (data.graph.hasEdge(i, j))
                    roadsFile << i << ',' << j << ',' << data.graph.getDistance(i, j) << ','
                              << data.graph.getRisk(i, j) << ',' << data.graph.isRoadBlocked(i, j) << '\n';
    }

    ofstream disastersFile(makePath(directory, "disasters.csv").c_str());

    if (!disastersFile.is_open()) success = false;
    else
    {
        disastersFile << "id,location_id,type,severity,active\n";

        for (int i = 0; i < data.disasters.getDisasterCount(); i++)
        {
            const Disaster& disaster = data.disasters.getDisasterAt(i);

            disastersFile << disaster.getDisasterId() << ',' << disaster.getLocationId() << ','
                          << disaster.getType() << ',' << disaster.getSeverity() << ','
                          << disaster.isActive() << '\n';
        }
    }

    return success;
}
