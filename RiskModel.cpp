#include "RiskModel.h"

int IncidentRiskModel::assess(int currentRisk, int source, int destination,const WeatherSnapshot[],const DisasterManager& disasters) const
{
    int incidentAdjustedRisk = disasters.getEffectiveRisk(currentRisk, source, destination);
    return incidentAdjustedRisk > 5 ? 5 : incidentAdjustedRisk;
}

int WeatherRiskModel::getWeatherAdjustment(const WeatherSnapshot& weather) const
{
    if (!weather.available)
    {
        return 0;
    }

    int adjustment = 0;
    if (weather.precipitationMm >= 30.0)
        adjustment += 3;
    else if (weather.precipitationMm >= 15.0)
        adjustment += 2;
    else if (weather.precipitationMm >= 5.0)
        adjustment += 1;

    if (weather.windKmh >= 80.0)
        adjustment += 3;
    else if (weather.windKmh >= 60.0)
        adjustment += 2;
    else if (weather.windKmh >= 40.0)
        adjustment += 1;

    return adjustment > 4 ? 4 : adjustment;
}

int WeatherRiskModel::assess(int currentRisk, int source, int destination, const WeatherSnapshot weather[], const DisasterManager&) const
{
    int sourceAdjustment = getWeatherAdjustment(weather[source]);
    int destinationAdjustment = getWeatherAdjustment(weather[destination]);
    int adjustment = sourceAdjustment > destinationAdjustment ? sourceAdjustment : destinationAdjustment;
    int adjustedRisk = currentRisk + adjustment;
    return adjustedRisk > 5 ? 5 : adjustedRisk;
}

