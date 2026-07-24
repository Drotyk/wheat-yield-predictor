#ifndef VarietyBonus_H
#define VarietyBonus_H

/**
 * @brief Додаткові характеристики та коефіцієнти чутливості сорту.
 * Використовує дефолтні ініціалізатори полів для уникнення конструкторів з великою кількістю параметрів.
 */
struct VarietyBonus {
    double soilBonus         = 0.0;
    double weatherBonus      = 0.0;
    double fertilizerBonus   = 0.0;
    double regionBonus       = 0.0;
    double droughtResistance = 5.0;
    double frostResistance   = 5.0;
    double protein           = 12.0;
    double gluten            = 25.0;
    double yieldPotential    = 1.0;

    VarietyBonus() = default;

    /// Конструктор для швидкого створення з основними бонусами (≤ 6 параметрів)
    VarietyBonus(double s, double w, double f, double r,
                 double dr = 5.0, double fr = 5.0)
        : soilBonus(s), weatherBonus(w), fertilizerBonus(f), regionBonus(r),
          droughtResistance(dr), frostResistance(fr) {}
};

#endif