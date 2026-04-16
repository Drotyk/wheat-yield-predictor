#ifndef FertilizerEffect_H
#define FertilizerEffect_H

#include <string>

class FertilizerEffect {
    std::string name;
    double increase;
public:
    FertilizerEffect(std::string n, double inc);
    std::string getName() const;
    double getIncrease() const;
};
#endif