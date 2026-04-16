#include "Region.h"

Region::Region(std::string n, double f) : name(std::move(n)), factor(f) {}
std::string Region::getName() const { return name; }
double Region::getFactor() const { return factor; }
