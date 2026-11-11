#include "Weather.h"

WeatherSnapshot::WeatherSnapshot()
{
    available = false;
    observedAt[0] = '\0';
    temperatureC = 0.0;
    precipitationMm = 0.0;
    windKmh = 0.0;
    weatherCode = -1;
}
