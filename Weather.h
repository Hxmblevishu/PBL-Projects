#ifndef WEATHER_H
#define WEATHER_H

struct WeatherSnapshot
{
    bool available;
    char observedAt[32];
    double temperatureC;
    double precipitationMm;
    double windKmh;
    int weatherCode;

    WeatherSnapshot();
};

#endif
