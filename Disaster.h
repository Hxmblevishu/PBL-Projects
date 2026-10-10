#ifndef DISASTER_H
#define DISASTER_H

const int MAX_DISASTERS = 50;

class Disaster
{
private:
    int disasterId;
    int locationId;
    char type[30];
    int severity;
    int active;

public:
    Disaster();
    Disaster(int id, int locationId, const char type[], int severity, int active);

    void setDisaster(int id, int locationId, const char type[], int severity, int active);

    int getDisasterId();
    int getLocationId();
    const char* getType();
    int getSeverity();
    int isActive();

    void setActive(int status);
    void display();
};

class DisasterManager
{
private:
    Disaster disasters[MAX_DISASTERS];
    int disasterCount;

public:
    DisasterManager();

    int getDisasterCount();
    bool addDisaster(int locationId, const char type[], int severity);
    void viewDisasters();
    void deactivateDisaster(int disasterId);

    int getEffectiveRisk(int baseRisk, int src, int dest);
    bool isRoadBlockedByDisaster(int src, int dest);
    Disaster* getDisaster(int index);
};

#endif
