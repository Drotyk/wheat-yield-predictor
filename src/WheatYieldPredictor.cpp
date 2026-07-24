#include "WheatYieldPredictor.h"
#include <algorithm>
#include <iostream>

WheatYieldPredictor::WheatYieldPredictor(WheatVariety v, SoilType s, FertilizerEffect f,
                                         std::vector<WeatherCondition> sp, std::vector<WeatherCondition> su,
                                         std::vector<WeatherCondition> au, std::vector<WeatherCondition> wi,
                                         Region r)
    : variety(std::move(v)), soil(std::move(s)), fertilizer(std::move(f)),
      spring(std::move(sp)), summer(std::move(su)), autumn(std::move(au)), winter(std::move(wi)),
      region(std::move(r)) {}

double WheatYieldPredictor::averageImpact(const std::vector<WeatherCondition>& v) {
    if (v.empty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (const auto& w : v) {
        sum += w.getImpact();
    }
    return sum / v.size();
}

double WheatYieldPredictor::calculateYield() const {
    double Y = variety.getAvgYield();
    VarietyBonus b = variety.getBonus();

    // --- Базові коефіцієнти ------------------------------------------------
    double Fsoil   = soil.getFactor();
    double fertInc = fertilizer.getIncrease(); // відсоток
    double Ffert   = 1.0 + fertInc / 100.0;
    double regionF = region.getFactor();        // відсоток (може бути від'ємним)
    double Fregion = 1.0 + regionF / 100.0;

    double weatherImpact = (averageImpact(spring) + averageImpact(summer) +
                            averageImpact(autumn) + averageImpact(winter)) / 4.0;
    double Fweather = 1.0 + weatherImpact / 100.0;

    // Стійкість сорту до посухи і морозу
    double Fvariety = 1.0 + ((b.droughtResistance + b.frostResistance) / 20.0) / 100.0;

    // --- Бонуси сорту: підсилюють чутливість до кожного фактора -----------
    // soilBonus    — % підсилення приросту/спаду від ґрунту
    // weatherBonus — % підсилення середнього погодного впливу
    // fertBonus    — % підсилення приросту від добрив
    // regionBonus  — % підсилення регіонального фактора
    double soilEffect    = (Fsoil - 1.0) * b.soilBonus / 100.0;        // Δ від ґрунту × чутливість
    double weatherEffect = (weatherImpact / 100.0) * b.weatherBonus / 100.0;
    double fertEffect    = (fertInc / 100.0) * b.fertilizerBonus / 100.0;
    double regionEffect  = (regionF / 100.0) * b.regionBonus / 100.0;
    double Fbonus        = 1.0 + soilEffect + weatherEffect + fertEffect + regionEffect;

    // yieldPotential — генетичний потенціал сорту (масштабувальний коефіцієнт)
    double Fpotential = b.yieldPotential;

    // Бонус стійкості до гербіцидів
    double delta = variety.isResistant() ? (0.05 * Y) : 0.0;

    double result = Y * Fsoil * Ffert * Fregion * Fweather * Fvariety * Fbonus * Fpotential + delta;
    result = std::min(result, variety.getMaxYield());
    result = std::max(result, variety.getMinYield());
    return result;
}

void WheatYieldPredictor::displayPrediction() const {
    VarietyBonus b = variety.getBonus();
    std::cout << "\n===== ПРОГНОЗ ВРОЖАЙНОСТІ =====\n";
    std::cout << "Сорт: " << variety.getName() << " (" << variety.getSpecies() << ")\n";
    std::cout << "Тип сівби: " << variety.getSowingType() << "\n";
    std::cout << "Ґрунт: " << soil.getName() << "\n";
    std::cout << "Добриво: " << fertilizer.getName() << "\n";
    std::cout << "Регіон: " << region.getName() << "\n";
    std::cout << "Стійкість до гербіцидів: " << (variety.isResistant() ? "так " : "ні ") << "\n";

    auto showSeason = [](const std::string& name, const std::vector<WeatherCondition>& conds) {
        std::cout << name << ": ";
        if (conds.empty()) std::cout << "—";
        else for (size_t idx = 0; idx < conds.size(); ++idx) {
            std::cout << conds[idx].getDescription();
            if (idx < conds.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
    };

    std::cout << "\nПОГОДНІ УМОВИ ЗА СЕЗОНАМИ:\n";
    showSeason(" Весна", spring);
    showSeason(" Літо", summer);
    showSeason(" Осінь", autumn);
    showSeason(" Зима", winter);

    std::cout << "--------------------------------\n";
    std::cout << "Прогнозована врожайність: " << calculateYield() << " т/га\n";
    std::cout << "Мінімальна: " << variety.getMinYield() << " | Середня: " << variety.getAvgYield()
              << " | Максимальна: " << variety.getMaxYield() << "\n";
    std::cout << "--------------------------------\n";
    std::cout << "Білок: " << b.protein << "% | Клейковина: " << b.gluten << "%\n";
    std::cout << "Посухостійкість: " << b.droughtResistance << "/10 | Зимостійкість: " << b.frostResistance << "/10\n";
    std::cout << "--------------------------------\n";
}
