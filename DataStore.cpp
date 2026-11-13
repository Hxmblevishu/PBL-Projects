#include "DataStore.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>

ProjectData::ProjectData()
{
    locationCount = 0;
}

static std::string makePath(const char directory[], const char fileName[])
{
    std::string path(directory);
    if (!path.empty() && path[path.length() - 1] != '/' && path[path.length() - 1] != '\\')
        path += '/';
    path += fileName;
    return path;
}

static bool readLocations(ProjectData& data, const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line); // header
    while (std::getline(file, line) && data.locationCount < MAX_LOCATIONS)
    {
        std::stringstream row(line);
        std::string idText, name;
        if (!std::getline(row, idText, ',') || !std::getline(row, name, ','))
            continue;

        int id = std::atoi(idText.c_str());
        if (id < 0 || id >= MAX_LOCATIONS || name.empty()) continue;

        data.locations[id].setLocation(id, name.c_str());
        if (id >= data.locationCount) data.locationCount = id + 1;
    }
    return data.locationCount > 0;
}

static void readRoads(ProjectData& data, const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        std::stringstream row(line);
        std::string fromText, toText, distanceText, riskText, blockedText;
        if (!std::getline(row, fromText, ',') || !std::getline(row, toText, ',') ||
            !std::getline(row, distanceText, ',') || !std::getline(row, riskText, ',') ||
            !std::getline(row, blockedText, ','))
            continue;
        int from = std::atoi(fromText.c_str());
        int to = std::atoi(toText.c_str());
        int distance = std::atoi(distanceText.c_str());
        int risk = std::atoi(riskText.c_str());
        int blocked = std::atoi(blockedText.c_str());
        if (from >= 0 && to >= 0 && from < data.locationCount && to < data.locationCount &&
            from != to && distance > 0 && risk >= 1 && risk <= 5)
            data.graph.addRoad(from, to, distance, risk, true);
        if (blocked && from >= 0 && to >= 0 && from < data.locationCount && to < data.locationCount)
            data.graph.blockRoad(from, to, true);
    }
}

static void readWeather(ProjectData& data, const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        std::stringstream row(line);
        std::string idText, observedAt, temperatureText, precipitationText, windText, codeText;
        if (!std::getline(row, idText, ',') || !std::getline(row, observedAt, ',') ||
            !std::getline(row, temperatureText, ',') || !std::getline(row, precipitationText, ',') ||
            !std::getline(row, windText, ',') || !std::getline(row, codeText, ','))
            continue;
        int id = std::atoi(idText.c_str());
        if (id < 0 || id >= data.locationCount) continue;
        WeatherSnapshot& snapshot = data.weather[id];
        snapshot.available = true;
        std::strncpy(snapshot.observedAt, observedAt.c_str(), sizeof(snapshot.observedAt) - 1);
        snapshot.observedAt[sizeof(snapshot.observedAt) - 1] = '\0';
        snapshot.temperatureC = std::atof(temperatureText.c_str());
        snapshot.precipitationMm = std::atof(precipitationText.c_str());
        snapshot.windKmh = std::atof(windText.c_str());
        snapshot.weatherCode = std::atoi(codeText.c_str());
    }
}

static void readDisasters(ProjectData& data, const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
        std::stringstream row(line);
        std::string idText, locationText, type, severityText, activeText;
        if (!std::getline(row, idText, ',') || !std::getline(row, locationText, ',') ||
            !std::getline(row, type, ',') || !std::getline(row, severityText, ',') ||
            !std::getline(row, activeText, ','))
            continue;
        int location = std::atoi(locationText.c_str());
        if (location >= 0 && location < data.locationCount)
            data.disasters.restoreDisaster(std::atoi(idText.c_str()), location, type.c_str(),
                                           std::atoi(severityText.c_str()), std::atoi(activeText.c_str()));
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
    std::ofstream locationsFile(makePath(directory, "locations.csv").c_str());
    if (!locationsFile.is_open()) success = false;
    else
    {
        locationsFile << "id,name\n";
        for (int i = 0; i < data.locationCount; i++)
            locationsFile << data.locations[i].getId() << ',' << data.locations[i].getName() << '\n';
    }

    std::ofstream roadsFile(makePath(directory, "roads.csv").c_str());
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

    std::ofstream disastersFile(makePath(directory, "disasters.csv").c_str());
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

