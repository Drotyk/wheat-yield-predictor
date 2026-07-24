#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include "WheatYieldPredictor.h"
#include "ConfigLoader.h"

using namespace std;

// Функція для очищення екрана термінала залежно від ОС (без використання макросів #define)
static void clearScreen() {
#ifdef _WIN32
    int res = system("cls");
#else
    int res = system("clear");
#endif
    (void)res;
}

// Шлях до файлу конфігурації відносно робочої директорії
static constexpr const char* CONFIG_PATH = "data/config.json";

// Безпечне зчитування цілого числа з cin.
// При некоректному введенні (не число) очищує стан потоку і повторює запит.
static int safeReadInt() {
    int value;
    while (true) {
        if (cin >> value) return value;
        cin.clear();                        // скинути failbit
        cin.ignore(10000, '\n');            // видалити некоректний рядок
        cout << "[Помилка] Введіть число: ";
    }
}

// Універсальний шаблон для вибору з меню
template<typename T, typename GetNameFunc>
int chooseFromList(const vector<T>& items, const string& title, GetNameFunc getName) {
    clearScreen();
    cout << "===== " << title << " =====\n";
    for (size_t idx = 0; idx < items.size(); ++idx)
        cout << idx + 1 << ". " << getName(items[idx]) << "\n";
    cout << "\n> ";
    int choice = safeReadInt();
    return max(1, min((int)items.size(), choice)) - 1;
}

// Вибір погодних умов для сезону
vector<WeatherCondition> chooseWeatherSeason(const string& season, const vector<WeatherCondition>& options) {
    clearScreen();
    cout << "\nОберіть погодні умови для сезону [" << season << "] (через пробіл, 0 — завершити):\n";
    for (size_t idx = 0; idx < options.size(); ++idx)
        cout << idx + 1 << ". " << options[idx].getDescription() << "\n";
    cout << "> ";

    vector<WeatherCondition> chosen;
    int index;
    while (true) {
        index = safeReadInt();
        if (index == 0) break;
        if (index >= 1 && index <= (int)options.size())
            chosen.push_back(options[index - 1]);
        else
            cout << "[Помилка] Невірний номер. Введіть від 1 до " << options.size() << " або 0 для завершення: ";
    }
    cin.ignore(10000, '\n');
    return chosen;
}

int main() {
    // Завантаження конфігурації
    AppConfig cfg;
    try {
        cfg = ConfigLoader::loadAll(CONFIG_PATH);
    } catch (const std::exception& e) {
        cerr << "\n[ПОМИЛКА] Не вдалося завантажити конфігурацію:\n  " << e.what() << "\n";
        cerr << "Переконайтеся, що файл '" << CONFIG_PATH << "' існує поруч з виконуваним файлом.\n";
        return 1;
    }

    // Вибір сорту
    int varIdx = chooseFromList(cfg.varieties, "Оберіть сорт пшениці",
        [](const WheatVariety& v){ return v.getName(); });
    WheatVariety variety = cfg.varieties[varIdx];

    // Вибір типу ґрунту
    int soilIdx = chooseFromList(cfg.soils, "Оберіть тип ґрунту",
        [](const SoilType& s){ return s.getName(); });
    SoilType soil = cfg.soils[soilIdx];

    // Вибір добрива
    int fertIdx = chooseFromList(cfg.fertilizers, "Оберіть тип добрива",
        [](const FertilizerEffect& f){ return f.getName(); });
    FertilizerEffect fertilizer = cfg.fertilizers[fertIdx];

    // Вибір агрокліматичного регіону
    int regionIdx = chooseFromList(cfg.regions, "Оберіть агрокліматичний регіон України",
        [](const Region& r){ return r.getName(); });
    Region region = cfg.regions[regionIdx];

    // Погодні умови
    auto spring = chooseWeatherSeason("Весна", cfg.weather);
    auto summer = chooseWeatherSeason("Літо",  cfg.weather);
    auto autumn = chooseWeatherSeason("Осінь", cfg.weather);
    auto winter = chooseWeatherSeason("Зима",  cfg.weather);

    // Прогноз
    WheatYieldPredictor predictor(variety, soil, fertilizer, spring, summer, autumn, winter, region);
    clearScreen();
    predictor.displayPrediction();

    cout << "\nНатисніть Enter, щоб завершити...";
    cin.ignore();
    cin.get();
    return 0;
}
