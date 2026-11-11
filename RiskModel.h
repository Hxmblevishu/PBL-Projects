#ifndef RISKMODEL_H
#define RISKMODEL_H

#include "Weather.h"
#include "Disaster.h"

class RiskModel
{
public:
    virtual ~RiskModel() {}
    virtual int assess(int currentRisk, int source, int destination,
                       const WeatherSnapshot weather[],
                       const DisasterManager& disasters) const = 0;
};

class IncidentRiskModel : public RiskModel
{
public:
    int assess(int currentRisk, int source, int destination,
               const WeatherSnapshot weather[],
               const DisasterManager& disasters) const;
};

class WeatherRiskModel : public RiskModel
{
private:
    int getWeatherAdjustment(const WeatherSnapshot& weather) const;

public:
    int assess(int currentRisk, int source, int destination,
               const WeatherSnapshot weather[],
               const DisasterManager& disasters) const;
};

#endif
