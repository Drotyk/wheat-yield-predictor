#include "ConfigLoader.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ─── Допоміжні парсери ────────────────────────────────────────────────────

static VarietyBonus parseBonus(const json& j) {
    VarietyBonus b;
    b.soilBonus         = j.value("soilBonus",         0.0);
    b.weatherBonus      = j.value("weatherBonus",      0.0);
    b.fertilizerBonus   = j.value("fertilizerBonus",   0.0);
    b.regionBonus       = j.value("regionBonus",       0.0);
    b.droughtResistance = j.value("droughtResistance", 5.0);
    b.frostResistance   = j.value("frostResistance",   5.0);
    b.protein           = j.value("protein",          12.0);
    b.gluten            = j.value("gluten",           25.0);
    b.yieldPotential    = j.value("yieldPotential",    1.0);
    return b;
}

static WheatVariety parseVariety(const json& j) {
    return WheatVariety(
        j.at("name")            .get<std::string>(),
        j.at("sowingType")      .get<std::string>(),
        j.at("minYield")        .get<double>(),
        j.at("avgYield")        .get<double>(),
        j.at("maxYield")        .get<double>(),
        j.at("herbicideResistant").get<bool>(),
        j.at("species")         .get<std::string>(),
        parseBonus(j.at("bonus"))
    );
}

static SoilType parseSoil(const json& j) {
    return SoilType(j.at("name").get<std::string>(), j.at("factor").get<double>());
}

static FertilizerEffect parseFertilizer(const json& j) {
    return FertilizerEffect(j.at("name").get<std::string>(), j.at("increase").get<double>());
}

static Region parseRegion(const json& j) {
    return Region(j.at("name").get<std::string>(), j.at("factor").get<double>());
}

static WeatherCondition parseWeather(const json& j) {
    return WeatherCondition(j.at("description").get<std::string>(), j.at("impact").get<double>());
}

// ─── Завантаження JSON-файлу ──────────────────────────────────────────────

static json readJson(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw ConfigException("Не вдалося відкрити файл конфігурації: " + path);
    json root;
    try {
        file >> root;
    } catch (const json::parse_error& e) {
        throw ConfigException(std::string("Помилка парсингу JSON: ") + e.what());
    }
    return root;
}

// ─── Валідація завантаженого конфігу ─────────────────────────────────

static void validate(const AppConfig& cfg) {
    // Перевірка на порожні списки
    if (cfg.varieties.empty())
        throw ConfigException("Конфіг: список \"varieties\" порожній");
    if (cfg.soils.empty())
        throw ConfigException("Конфіг: список \"soils\" порожній");
    if (cfg.fertilizers.empty())
        throw ConfigException("Конфіг: список \"fertilizers\" порожній");
    if (cfg.regions.empty())
        throw ConfigException("Конфіг: список \"regions\" порожній");
    if (cfg.weather.empty())
        throw ConfigException("Конфіг: список \"weather\" порожній");

    // Перевірка меж врожайності для кожного сорту
    for (const auto& v : cfg.varieties) {
        if (v.getMinYield() < 0.0)
            throw ConfigException("Сорт \"" + v.getName() + "\": minYield не може бути від'ємним");
        if (v.getMinYield() > v.getMaxYield())
            throw ConfigException("Сорт \"" + v.getName() + "\": minYield > maxYield");
        if (v.getAvgYield() < v.getMinYield() || v.getAvgYield() > v.getMaxYield())
            throw ConfigException("Сорт \"" + v.getName() + "\": avgYield повинен бути між minYield і maxYield");
    }
}

// ─── Публічні методи ConfigLoader ────────────────────────────────────────

AppConfig ConfigLoader::loadAll(const std::string& path) {
    json root = readJson(path);
    AppConfig cfg;

    for (const auto& v : root.at("varieties"))   cfg.varieties.push_back(parseVariety(v));
    for (const auto& s : root.at("soils"))        cfg.soils.push_back(parseSoil(s));
    for (const auto& f : root.at("fertilizers"))  cfg.fertilizers.push_back(parseFertilizer(f));
    for (const auto& r : root.at("regions"))      cfg.regions.push_back(parseRegion(r));
    for (const auto& w : root.at("weather"))      cfg.weather.push_back(parseWeather(w));

    validate(cfg); // перевірка порожніх списків та коректності меж урожайності
    return cfg;
}

std::vector<WheatVariety> ConfigLoader::loadVarieties(const std::string& path) {
    json root = readJson(path);
    std::vector<WheatVariety> result;
    for (const auto& v : root.at("varieties")) result.push_back(parseVariety(v));
    return result;
}

std::vector<SoilType> ConfigLoader::loadSoils(const std::string& path) {
    json root = readJson(path);
    std::vector<SoilType> result;
    for (const auto& s : root.at("soils")) result.push_back(parseSoil(s));
    return result;
}

std::vector<FertilizerEffect> ConfigLoader::loadFertilizers(const std::string& path) {
    json root = readJson(path);
    std::vector<FertilizerEffect> result;
    for (const auto& f : root.at("fertilizers")) result.push_back(parseFertilizer(f));
    return result;
}

std::vector<Region> ConfigLoader::loadRegions(const std::string& path) {
    json root = readJson(path);
    std::vector<Region> result;
    for (const auto& r : root.at("regions")) result.push_back(parseRegion(r));
    return result;
}

std::vector<WeatherCondition> ConfigLoader::loadWeather(const std::string& path) {
    json root = readJson(path);
    std::vector<WeatherCondition> result;
    for (const auto& w : root.at("weather")) result.push_back(parseWeather(w));
    return result;
}
