#include "Disaster.h"
#include <iostream>
#include <cstring>

Disaster::Disaster()
{
    disasterId = -1;
    locationId = -1;
    type[0] = '\0';
    severity = 0;
    active = 0;
}

Disaster::Disaster(int id, int locationId, const char type[], int severity, int active)
{
    this->disasterId = id;
    this->locationId = locationId;
    this->severity = severity;
    this->active = active;

    for (int i = 0; i < 29 && type[i] != '\0'; i++)
    {
        this->type[i] = type[i];
    }
    this->type[29] = '\0';
}

void Disaster::setDisaster(int id, int locationId, const char type[], int severity, int active)
{
    this->disasterId = id;
    this->locationId = locationId;
    this->severity = severity;
    this->active = active;

    for (int i = 0; i < 29 && type[i] != '\0'; i++)
    {
        this->type[i] = type[i];
    }
    this->type[29] = '\0';
}

int Disaster::getDisasterId()
{
    return disasterId;
}

int Disaster::getLocationId()
{
    return locationId;
}

const char* Disaster::getType()
{
    return type;
}

int Disaster::getSeverity()
{
    return severity;
}

int Disaster::isActive()
{
    return active;
}

void Disaster::setActive(int status)
{
    active = status;
}

void Disaster::display()
{
    std::cout << "Disaster ID : " << disasterId << "\n"
              << "Location ID : " << locationId << "\n"
              << "Type        : " << type << "\n"
              << "Severity    : " << severity << "\n"
              << "Status      : " << (active ? "Active" : "Inactive") << "\n";
}

DisasterManager::DisasterManager()
{
    disasterCount = 0;
}

int DisasterManager::getDisasterCount()
{
    return disasterCount;
}

bool DisasterManager::addDisaster(int locationId, const char type[], int severity)
{
    if (disasterCount >= MAX_DISASTERS || locationId < 0 || severity < 1 || severity > 5)
    {
        return false;
    }

    disasters[disasterCount].setDisaster(disasterCount + 1, locationId, type, severity, 1);
    disasterCount++;
    return true;
}

void DisasterManager::viewDisasters()
{
    std::cout << "\n========== DISASTERS ==========\n";

    if (disasterCount == 0)
    {
        std::cout << "No disasters recorded.\n";
        return;
    }

    for (int i = 0; i < disasterCount; i++)
    {
        disasters[i].display();
        std::cout << "------------------------------\n";
    }
}

void DisasterManager::deactivateDisaster(int disasterId)
{
    for (int i = 0; i < disasterCount; i++)
    {
        if (disasters[i].getDisasterId() == disasterId)
        {
            disasters[i].setActive(0);
            return;
        }
    }
}

int DisasterManager::getEffectiveRisk(int baseRisk, int src, int dest)
{
    int effectiveRisk = baseRisk;

    for (int i = 0; i < disasterCount; i++)
    {
        if (disasters[i].isActive())
        {
            int affectedLocation = disasters[i].getLocationId();
            if (affectedLocation == src || affectedLocation == dest)
            {
                effectiveRisk += disasters[i].getSeverity();
            }
        }
    }

    return effectiveRisk;
}

bool DisasterManager::isRoadBlockedByDisaster(int src, int dest)
{
    for (int i = 0; i < disasterCount; i++)
    {
        if (disasters[i].isActive() && disasters[i].getSeverity() >= 5)
        {
            int affectedLocation = disasters[i].getLocationId();
            if (affectedLocation == src || affectedLocation == dest)
            {
                return true;
            }
        }
    }

    return false;
}
