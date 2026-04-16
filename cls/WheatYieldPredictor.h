#ifndef WHEAT_YIELD_PREDICTOR_H
#define WHEAT_YIELD_PREDICTOR_H

#include <vector>
#include <string>
#include <iostream>
#include "WheatVariety.h"
#include "SoilType.h"
#include "FertilizerEffect.h"
#include "WeatherCondition.h"
#include "Region.h"

class WheatYieldPredictor {
    WheatVariety variety;
    SoilType soil;
    FertilizerEffect fertilizer;
    std::vector<WeatherCondition> spring, summer, autumn, winter;
    Region region;
public:
    WheatYieldPredictor(WheatVariety v, SoilType s, FertilizerEffect f,
                        std::vector<WeatherCondition> sp, std::vector<WeatherCondition> su,
                        std::vector<WeatherCondition> au, std::vector<WeatherCondition> wi,
                        Region r);
    static double averageImpact(const std::vector<WeatherCondition>& v);
    double calculateYield();
    void displayPrediction();
};

#endif
