#ifndef LOCATION_H
#define LOCATION_H

class Location
{
private:
    int id;
    char name[30];

public:
    Location();
    Location(int id, const char name[]);

    void setLocation(int id, const char name[]);

    int getId();
    const char* getName();

    void display();
};

#endif
