#ifndef WheatVariety_H
#define WheatVariety_H

#include <string>
#include "VarietyBonus.h"

class WheatVariety {
    std::string name;
    std::string sowingType;
    std::string species;
    double min_yield;
    double avg_yield;
    double max_yield;
    bool herbicide_resistant;
    VarietyBonus bonus;
public:
    WheatVariety(std::string n, std::string type, double minY, double avgY, double maxY,
                 bool resistant, std::string s, VarietyBonus b);
    std::string getName() const;
    std::string getSowingType() const;
    std::string getSpecies() const;
    double getMinYield() const;
    double getAvgYield() const;
    double getMaxYield() const;
    bool isResistant() const;
    VarietyBonus getBonus() const;
    void setResistant(bool r);
};

#endif
