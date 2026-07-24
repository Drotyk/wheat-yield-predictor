/**
 * Юніт-тести для Wheat Yield Predictor.
 * Фреймворк: Catch2 v3 (системний пакет).
 *
 * Запуск: make test
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "WheatYieldPredictor.h"
#include "ConfigLoader.h"

using Catch::Approx;

// ─── Допоміжні фабрики ────────────────────────────────────────────────────

/**
 * Фабрика сорту. Діапазон [min, max] навмисно широкий (0..100 т/га)
 * щоб clamp не маскував різниці між факторами у порівняльних тестах.
 * Для тестів clamping передаємо явний вузький max.
 */
static WheatVariety makeVariety(double minY = 0.0, double avgY = 6.0, double maxY = 100.0,
                                bool resistant = false, VarietyBonus bonus = VarietyBonus()) {
    return WheatVariety("Тест", "озима", minY, avgY, maxY, resistant, "м'яка", bonus);
}

/// Нейтральний ґрунт (factor=1.0, без впливу)
static SoilType makeSoilNeutral() { return SoilType("Лучний",   1.00); }
static SoilType makeSoilFertile() { return SoilType("Чорнозем", 1.10); }
static SoilType makeSoilPoor()    { return SoilType("Піщаний",  0.85); }

/// Добрива
static FertilizerEffect makeNoFert()  { return FertilizerEffect("Без добрив",   0.0); }
static FertilizerEffect makeNPKFert() { return FertilizerEffect("NPK 16-16-16", 8.0); }


/// Погода
static std::vector<WeatherCondition> noWeather() { return {}; }
static std::vector<WeatherCondition> rainy()     { return { WeatherCondition("дощова",  2.5) }; }
static std::vector<WeatherCondition> hot()       { return { WeatherCondition("спекотна",-3.0) }; }
static std::vector<WeatherCondition> dry()       { return { WeatherCondition("суха",   -2.0) }; }

// ─── Хелпер для швидкого розрахунку ─────────────────────────────────────

static double calcYield(WheatVariety v, SoilType s, FertilizerEffect f,
                        std::vector<WeatherCondition> w = {},
                        Region r = Region("Нейтральний", 0.0)) {
    WheatYieldPredictor p(v, s, f, w, w, w, w, r);
    return p.calculateYield();
}

// ─── Тести averageImpact ──────────────────────────────────────────────────

TEST_CASE("WeatherAverage: порожній вектор повертає 0.0", "[weather]") {
    auto empty = noWeather();
    REQUIRE(WheatYieldPredictor::averageImpact(empty) == Approx(0.0));
}

TEST_CASE("WeatherAverage: середнє двох значень коректне", "[weather]") {
    std::vector<WeatherCondition> conds = {
        WeatherCondition("a", 2.0),
        WeatherCondition("b", 4.0)
    };
    REQUIRE(WheatYieldPredictor::averageImpact(conds) == Approx(3.0));
}

// ─── Тести calculateYield ─────────────────────────────────────────────────

TEST_CASE("CorrectBase: нейтральні умови — врожайність близька до avg", "[yield]") {
    // Усі коефіцієнти = 1.0 → результат ≈ avg_yield = 6.0
    double y = calcYield(makeVariety(), makeSoilNeutral(), makeNoFert());
    REQUIRE(y == Approx(6.0).epsilon(0.05)); // ±5%
}

TEST_CASE("ClampToMin: дуже погані умови — результат >= min_yield", "[yield][clamp]") {
    // Піщаний ґрунт, без добрив, посушливий регіон, спекотна погода → clamp до min
    WheatYieldPredictor p(makeVariety(4.0, 6.0, 100.0), makeSoilPoor(), makeNoFert(),
                          hot(), hot(), hot(), hot(),
                          Region("Посушливий", -50.0));
    REQUIRE(p.calculateYield() >= 4.0);
}

TEST_CASE("ClampToMax: відмінні умови — результат <= max_yield", "[yield][clamp]") {
    // max=7.0 — завідомо нижче ніж реальний результат з усіма бонусами
    WheatYieldPredictor p(makeVariety(0.0, 6.0, 7.0, true), makeSoilFertile(), makeNPKFert(),
                          rainy(), rainy(), rainy(), rainy(),
                          Region("Лісостеп", 5.0));
    REQUIRE(p.calculateYield() <= 7.0);
}

TEST_CASE("FertilizerEffect: добрива підвищують врожайність", "[yield][fertilizer]") {
    double withFert    = calcYield(makeVariety(), makeSoilNeutral(), makeNPKFert());
    double withoutFert = calcYield(makeVariety(), makeSoilNeutral(), makeNoFert());
    REQUIRE(withFert > withoutFert);
}

TEST_CASE("HerbicideBonus: стійкий сорт дає більшу врожайність", "[yield][variety]") {
    // Явно великий max щоб clamp не вирівнював обидва значення
    double resistant    = calcYield(makeVariety(0.0, 6.0, 100.0, true),  makeSoilNeutral(), makeNoFert());
    double notResistant = calcYield(makeVariety(0.0, 6.0, 100.0, false), makeSoilNeutral(), makeNoFert());
    REQUIRE(resistant > notResistant);
}

TEST_CASE("SoilEffect: родючий ґрунт дає більшу врожайність ніж бідний", "[yield][soil]") {
    double fertile = calcYield(makeVariety(), makeSoilFertile(), makeNoFert());
    double poor    = calcYield(makeVariety(), makeSoilPoor(),    makeNoFert());
    REQUIRE(fertile > poor);
}

TEST_CASE("WeatherPositive: дощова погода підвищує врожайність", "[yield][weather]") {
    double withRain    = calcYield(makeVariety(), makeSoilNeutral(), makeNoFert(), rainy());
    double withNoRain  = calcYield(makeVariety(), makeSoilNeutral(), makeNoFert(), noWeather());
    REQUIRE(withRain > withNoRain);
}

TEST_CASE("WeatherNegative: суха погода знижує врожайність", "[yield][weather]") {
    double neutral = calcYield(makeVariety(), makeSoilNeutral(), makeNoFert(), noWeather());
    double dry_w   = calcYield(makeVariety(), makeSoilNeutral(), makeNoFert(), dry());
    REQUIRE(neutral > dry_w);
}

// ─── Тести ConfigLoader ───────────────────────────────────────────────────

TEST_CASE("ConfigLoad: завантаження config.json без винятків", "[config]") {
    REQUIRE_NOTHROW(ConfigLoader::loadAll("data/config.json"));
}

TEST_CASE("ConfigCounts: конфіг містить очікувану кількість записів", "[config]") {
    AppConfig cfg = ConfigLoader::loadAll("data/config.json");
    REQUIRE(cfg.varieties.size()   == 3);
    REQUIRE(cfg.soils.size()       == 4);
    REQUIRE(cfg.fertilizers.size() == 4);
    REQUIRE(cfg.regions.size()     == 5);
    REQUIRE(cfg.weather.size()     == 6);
}

TEST_CASE("ConfigLoad: неіснуючий файл кидає runtime_error", "[config]") {
    REQUIRE_THROWS_AS(ConfigLoader::loadAll("data/nonexistent.json"), std::runtime_error);
}
