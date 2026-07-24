#include "ConfigLoader.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ─── Допоміжні парсери ────────────────────────────────────────────────────

static VarietyBonus parseBonus(const json& j) {
    return VarietyBonus(
        j.value("soilBonus",        0.0),
        j.value("weatherBonus",     0.0),
        j.value("fertilizerBonus",  0.0),
        j.value("regionBonus",      0.0),
        j.value("droughtResistance",5.0),
        j.value("frostResistance",  5.0),
        j.value("protein",          12.0),
        j.value("gluten",           25.0),
        j.value("yieldPotential",   1.0)
    );
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
        throw std::runtime_error("Не вдалося відкрити файл конфігурації: " + path);
    json root;
    try {
        file >> root;
    } catch (const json::parse_error& e) {
        throw std::runtime_error(std::string("Помилка парсингу JSON: ") + e.what());
    }
    return root;
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
