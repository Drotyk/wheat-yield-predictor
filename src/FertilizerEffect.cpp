#include "FertilizerEffect.h"

FertilizerEffect::FertilizerEffect(std::string n, double inc) : name(std::move(n)), increase(inc) {}
std::string FertilizerEffect::getName() const { return name; }
double FertilizerEffect::getIncrease() const { return increase; }
