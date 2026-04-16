# Wheat Predictor — Split Version

## Structure
```
WheatPredictorSplit/
  include/
    FertilizerEffect.h
    Region.h
    SoilType.h
    VarietyBonus.h
    WeatherCondition.h
    WheatVariety.h
    WheatYieldPredictor.h
  src/
    FertilizerEffect.cpp
    Region.cpp
    SoilType.cpp
    WeatherCondition.cpp
    WheatVariety.cpp
    WheatYieldPredictor.cpp
  Makefile
  example_main.cpp
```

## Build with your existing `main.cpp`
Place your `main.cpp` anywhere and run:
```bash
cd WheatPredictorSplit
make MAIN=/full/path/to/your/main.cpp
```

or compile manually:
```bash
g++ -std=c++17 -Iinclude src/*.cpp /full/path/to/your/main.cpp -o wheat_predictor
```

## Quick test with the included example
```bash
cd WheatPredictorSplit
make MAIN=example_main.cpp
./wheat_predictor
```
