#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <vector>
#include <stdexcept>
#include "WheatVariety.h"
#include "SoilType.h"
#include "FertilizerEffect.h"
#include "WeatherCondition.h"
#include "Region.h"

/**
 * @brief Спеціалізований клас виключення для помилок завантаження та валідації конфігурації.
 */
class ConfigException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/// Агрегована структура, що містить усі дані, завантажені з конфіг-файлу.
struct AppConfig {
    std::vector<WheatVariety>     varieties;
    std::vector<SoilType>         soils;
    std::vector<FertilizerEffect> fertilizers;
    std::vector<Region>           regions;
    std::vector<WeatherCondition> weather;
};

/**
 * @brief Статичний завантажувач конфігурації з JSON-файлу.
 *
 * Використовує бібліотеку nlohmann/json (header-only).
 * Якщо файл не знайдено або JSON некоректний — кидає ConfigException.
 */
class ConfigLoader {
public:
    ConfigLoader() = delete;

    /// Завантажити всі дані з одного JSON-файлу.
    static AppConfig loadAll(const std::string& path);

    /// Допоміжні методи для завантаження окремих секцій.
    static std::vector<WheatVariety>     loadVarieties  (const std::string& path);
    static std::vector<SoilType>         loadSoils      (const std::string& path);
    static std::vector<FertilizerEffect> loadFertilizers(const std::string& path);
    static std::vector<Region>           loadRegions    (const std::string& path);
    static std::vector<WeatherCondition> loadWeather    (const std::string& path);
};

#endif // CONFIG_LOADER_H
