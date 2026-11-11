#ifndef LOCATION_H
#define LOCATION_H

class Location
{
private:
    int id;
    char name[64];

public:
    Location();
    Location(int id, const char name[]);

    void setLocation(int id, const char name[]);

    int getId() const;
    const char* getName() const;

};

#endif

