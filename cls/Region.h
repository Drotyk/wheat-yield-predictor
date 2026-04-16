#ifndef Region_H
#define Region_H

#include <string>

class Region {
    std::string name;
    double factor;
public:
    Region(std::string n, double f);
    std::string getName() const;
    double getFactor() const;
};
#endif 