#include "WeatherCondition.h"

WeatherCondition::WeatherCondition(std::string d, double i) : description(std::move(d)), impact(i) {}
std::string WeatherCondition::getDescription() const { return description; }
double WeatherCondition::getImpact() const { return impact; }
