#ifndef SoilType_H
#define SoilType_H

#include <string>

class SoilType {
    std::string name;
    double factor;
public:
    SoilType(std::string n, double f);
    std::string getName() const;
    double getFactor() const;
};

#endif
