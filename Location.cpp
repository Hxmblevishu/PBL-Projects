#include "Location.h"
#include <iostream>
#include <cstring>

Location::Location()
{
    id = -1;
    name[0] = '\0';
}

Location::Location(int id, const char name[])
{
    setLocation(id, name);
}

void Location::setLocation(int id, const char name[])
{
    this->id = id;

    for (int i = 0; i < 29 && name[i] != '\0'; i++)
    {
        this->name[i] = name[i];
    }

    this->name[29] = '\0';
}

int Location::getId()
{
    return id;
}

const char* Location::getName()
{
    return name;
}

void Location::display()
{
    std::cout << "Location ID   : " << id << "\n"
              << "Location Name : " << name << "\n";
}
