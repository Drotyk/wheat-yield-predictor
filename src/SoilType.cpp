#include "SoilType.h"

SoilType::SoilType(std::string n, double f) : name(std::move(n)), factor(f) {}
std::string SoilType::getName() const { return name; }
double SoilType::getFactor() const { return factor; }
