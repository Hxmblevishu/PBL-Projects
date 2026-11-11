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

    int getDisasterId() const;
    int getLocationId() const;
    const char* getType() const;
    int getSeverity() const;
    int isActive() const;

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

    int getDisasterCount() const;
    bool addDisaster(int locationId, const char type[], int severity);
    bool restoreDisaster(int id, int locationId, const char type[], int severity, int active);
    const Disaster& getDisasterAt(int index) const;
    void viewDisasters();
    void deactivateDisaster(int disasterId);

    int getEffectiveRisk(int baseRisk, int src, int dest) const;
    bool isRoadBlockedByDisaster(int src, int dest) const;
};

#endif
