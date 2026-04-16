#include "WheatVariety.h"

WheatVariety::WheatVariety(std::string n, std::string type, double minY, double avgY, double maxY,
                           bool resistant, std::string s, VarietyBonus b)
    : name(std::move(n)), sowingType(std::move(type)), species(std::move(s)),
      min_yield(minY), avg_yield(avgY), max_yield(maxY),
      herbicide_resistant(resistant), bonus(b) {}

std::string WheatVariety::getName() const { return name; }
std::string WheatVariety::getSowingType() const { return sowingType; }
std::string WheatVariety::getSpecies() const { return species; }
double WheatVariety::getMinYield() const { return min_yield; }
double WheatVariety::getAvgYield() const { return avg_yield; }
double WheatVariety::getMaxYield() const { return max_yield; }
bool WheatVariety::isResistant() const { return herbicide_resistant; }
VarietyBonus WheatVariety::getBonus() const { return bonus; }
void WheatVariety::setResistant(bool r) { herbicide_resistant = r; }
