#ifndef VarietyBonus_H
#define VarietyBonus_H

#include <cstddef>

struct VarietyBonus {
    double soilBonus;
    double weatherBonus;
    double fertilizerBonus;
    double regionBonus;
    double droughtResistance;
    double frostResistance;
    double protein;
    double gluten;
    double yieldPotential;

    explicit VarietyBonus(double s = 0, double w = 0, double f = 0, double r = 0,
                          double dr = 5, double fr = 5, double p = 12, double g = 25, double y = 70)
        : soilBonus(s), weatherBonus(w), fertilizerBonus(f), regionBonus(r),
          droughtResistance(dr), frostResistance(fr),
          protein(p), gluten(g), yieldPotential(y) {}
};

#endif 