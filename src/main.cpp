#include <iostream>
#include "TrainingData.hpp"
#include "Test.hpp"
#include "UI.hpp"
#include "Matrix.hpp"

bool debug = 1;

int main()
{
    Perceptron model(14, 0.1);

    std::vector<TrainingSample> dataset = GetTrainingSamples();
    
    std::vector<TrainingSample> train_set;
    std::vector<TrainingSample> test_set;

    StratisfiedSplit(dataset, train_set, test_set, 0.8);

    std::cout << "Dataset split successfully: \n";
    std::cout << "Total dataset size: " << dataset.size() << std::endl;
    std::cout << "Training set size: " << train_set.size() << std::endl;
    std::cout << "Testing set size: " << test_set.size() << std::endl << std::endl;

    model.Train(train_set, 150, 16);
    
    if (debug) {
        model.PrintWeights();
        RunTests(model, test_set);
        return 0;
    }
    

    std::vector<double> input = StartUI();
    if (model.Predict(input))
        std::cout << "GO: можно идти в зал.\n";
    else
        std::cout << "REST: сегодня лучше восстановиться.\n";

    return 0;
}
// 0  sleep_low              — мало сна
// 1  sleep_normal           — нормальный сон
// 2  sleep_high             — много сна

// 3  state_bad              — ватное / плохое состояние
// 4  state_normal           — нормальное состояние
// 5  state_good             — хорошее состояние

// 6  heavy_yesterday        — вчера была тяжёлая тренировка
// 7  ate                    — нормально ел
// 8  hydrated               — нормально пил воду
// 9  strong_muscle_soreness — сильная крепатура

// 10 mood_bad               — плохое настроение
// 11 mood_normal            — нормальное настроение
// 12 mood_good              — хорошее настроение

// 13 time_for_recovery      — будет время восстановиться