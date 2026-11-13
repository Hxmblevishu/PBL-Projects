#ifndef DATASTORE_H
#define DATASTORE_H

#include "Location.h"
#include "Graph.h"
#include "Disaster.h"
#include "Weather.h"

const int MAX_LOCATIONS = MAX_VERTICES;

struct ProjectData
{
    Location locations[MAX_LOCATIONS];
    int locationCount;
    Graph graph;
    DisasterManager disasters;
    WeatherSnapshot weather[MAX_LOCATIONS];

    ProjectData();
};

class CsvDataStore
{
public:
    bool load(ProjectData& data, const char directory[]);
    bool save(const ProjectData& data, const char directory[]);
};

#endif
