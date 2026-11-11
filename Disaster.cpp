#include "Disaster.h"
#include "ConsoleUI.h"
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

int Disaster::getDisasterId() const
{
    return disasterId;
}

int Disaster::getLocationId() const
{
    return locationId;
}

const char* Disaster::getType() const
{
    return type;
}

int Disaster::getSeverity() const
{
    return severity;
}

int Disaster::isActive() const
{
    return active;
}

void Disaster::setActive(int status)
{
    active = status;
}

void Disaster::display()
{
    ConsoleUI ui;
    ui.setColor(ConsoleUI::PINK); std::cout << "  INCIDENT " << disasterId << "\n";
    ui.setColor(ConsoleUI::WHITE); std::cout << "  Place ID: " << locationId << "\n";
    std::cout << "  Situation: " << type << "\n";
    ui.setColor(severity >= 4 ? ConsoleUI::RED : ConsoleUI::PINK);
    std::cout << "  Severity: " << severity << "/5\n";
    ui.setColor(active ? ConsoleUI::RED : ConsoleUI::LIGHT_BLUE);
    std::cout << "  Status: " << (active ? "ACTIVE" : "CLOSED") << "\n";
    ui.resetColor();
}

DisasterManager::DisasterManager()
{
    disasterCount = 0;
}

int DisasterManager::getDisasterCount() const
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

bool DisasterManager::restoreDisaster(int id, int locationId, const char type[], int severity, int active)
{
    if (disasterCount >= MAX_DISASTERS || id <= 0 || locationId < 0 || severity < 1 || severity > 5)
    {
        return false;
    }

    disasters[disasterCount].setDisaster(id, locationId, type, severity, active ? 1 : 0);
    disasterCount++;
    return true;
}

const Disaster& DisasterManager::getDisasterAt(int index) const
{
    return disasters[index];
}

void DisasterManager::viewDisasters()
{
    ConsoleUI ui;
    ui.setColor(ConsoleUI::PINK);
    std::cout << "\n  DISASTER INFORMATION\n";
    ui.divider('=');
    ui.resetColor();

    if (disasterCount == 0)
    {
        ui.setColor(ConsoleUI::LIGHT_BLUE);
        std::cout << "  No incidents have been recorded.\n";
        ui.resetColor();
        return;
    }

    for (int i = 0; i < disasterCount; i++)
    {
        disasters[i].display();
        ui.divider();
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

int DisasterManager::getEffectiveRisk(int baseRisk, int src, int dest) const
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

bool DisasterManager::isRoadBlockedByDisaster(int src, int dest) const
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
