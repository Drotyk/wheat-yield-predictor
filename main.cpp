#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "WheatYieldPredictor.h"

using namespace std;

#ifdef _WIN32
#define CLEAR "cls"
#else 
#define CLEAR "clear"
#endif

// універсальний шаблон для вибору з меню
template<typename T, typename GetNameFunc>
int chooseFromList(const vector<T>& items, const string& title, GetNameFunc getName) {
    system(CLEAR);
    cout << "===== " << title << " =====\n";
    for (size_t idx = 0; idx < items.size(); ++idx)
        cout << idx + 1 << ". " << getName(items[idx]) << "\n";
    cout << "\n> ";
    int choice;
    cin >> choice;
    return max(1, min((int)items.size(), choice)) - 1;
}

// вибір погодних умов для сезону
vector<WeatherCondition> chooseWeatherSeason(const string& season, const vector<WeatherCondition>& options) {
    system(CLEAR);
    cout << "\nОберіть погодні умови для сезону [" << season << "] (через пробіл, 0 — завершити):\n";
    for (size_t idx = 0; idx < options.size(); ++idx)
        cout << idx + 1 << ". " << options[idx].getDescription() << "\n";
    cout << "> ";

    vector<WeatherCondition> chosen;
    int index;
    while (cin >> index && index != 0) {
        if (index >= 1 && index <= (int)options.size())
            chosen.push_back(options[index - 1]);
    }
    cin.clear(); cin.ignore(10000, '\n');
    return chosen;
}

int main() {
    //  Вибір сорту
    vector<WheatVariety> varieties = {
        {"Подолянка", "озима", 4, 6, 9, true, "м'яка", VarietyBonus(0,0,0,0,5,5,12,25,70)},
        {"Краєвид", "яра", 3.5, 5.5, 8.5, false, "тверда", VarietyBonus(0,0,0,0,6,6,13,27,75)},
        {"Скаген", "озима", 4.2, 6.5, 9.5, true, "м'яка", VarietyBonus(0,0,0,0,7,7,14,28,80)}
    };
    int varIdx = chooseFromList(varieties, "Оберіть сорт пшениці", [](const WheatVariety& v){ return v.getName(); });
    WheatVariety variety = varieties[varIdx];

    //  Вибір типу ґрунту
    vector<SoilType> soils = {
        {"Чорнозем", 1.10},
        {"Суглинок", 0.95},
        {"Піщаний", 0.85},
        {"Лучний", 1.00}
    };
    int soilIdx = chooseFromList(soils, "Оберіть тип ґрунту", [](const SoilType& s){ return s.getName(); });
    SoilType soil = soils[soilIdx];

    //  Вибір добрива
    vector<FertilizerEffect> fertilizers = {
        {"Без добрив", 0},
        {"NPK 16-16-16", 8},
        {"Амофос", 5},
        {"Карбамід", 10}
    };
    int fertIdx = chooseFromList(fertilizers, "Оберіть тип добрива", [](const FertilizerEffect& f){ return f.getName(); });
    FertilizerEffect fertilizer = fertilizers[fertIdx];

    //  Вибір агрокліматичного регіону (підзони)
    vector<Region> regions = {
        {"Полісся", -1.0},          // прохолодна, волога зона
        {"Лісостеп", 1.5},          // оптимальна для вирощування озимої пшениці
        {"Степ", 2.0},              // сухіша, але врожайна при добрій агротехніці
        {"Південний степ", -2.0},   // посушливий, ризикований регіон
        {"Карпати", -1.5}           // гірський, обмежені площі
    };
    int regionIdx = chooseFromList(regions, "Оберіть агрокліматичний регіон України", [](const Region& r){ return r.getName(); });
    Region region = regions[regionIdx];

    //  Погодні умови
    vector<WeatherCondition> weatherOptions = {
        {"дощова", 2.5},
        {"спекотна", -3.0},
        {"помірна", 1.0},
        {"суха", -2.0},
        {"м'яка", 1.5},
        {"вітряна", -1.5}
    };

    auto spring = chooseWeatherSeason("Весна", weatherOptions);
    auto summer = chooseWeatherSeason("Літо", weatherOptions);
    auto autumn = chooseWeatherSeason("Осінь", weatherOptions);
    auto winter = chooseWeatherSeason("Зима", weatherOptions);

    //  Прогноз
    WheatYieldPredictor predictor(variety, soil, fertilizer, spring, summer, autumn, winter, region);
    system(CLEAR);
    predictor.displayPrediction();

    cout << "\nНатисніть Enter, щоб завершити...";
    cin.ignore();
    cin.get();
    return 0;
}
