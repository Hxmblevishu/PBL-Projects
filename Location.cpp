#include "Location.h"

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
    int i = 0;
    while (i < 63 && name != 0 && name[i] != '\0')
    {
        this->name[i] = name[i];
        i++;
    }
    this->name[i] = '\0';
}

int Location::getId() const
{
    return id;
}

const char* Location::getName() const
{
    return name;
}
