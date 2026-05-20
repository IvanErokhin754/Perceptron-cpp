#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <algorithm>
#include <random>
#include "Perceptron.hpp"

inline void StratisfiedSplit(const MutableArraySequence<TrainingSample>& dataset, MutableArraySequence<TrainingSample>& train_set, MutableArraySequence<TrainingSample>& test_set, double train_ratio = 0.8) {

    MutableArraySequence<TrainingSample> class_0;
    MutableArraySequence<TrainingSample> class_1;

    for (const auto& sample : dataset) {
        if (sample.target == 0)
            class_0.push_back(sample);
        else
            class_1.push_back(sample);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(class_0.begin(), class_0.end(), g);
    std::shuffle(class_1.begin(), class_1.end(), g);

    size_t train_size_0 = static_cast<size_t>(class_0.size() * train_ratio);
    size_t train_size_1 = static_cast<size_t>(class_1.size() * train_ratio);

    train_set.insert(train_set.end(), class_0.begin(), class_0.begin() + train_size_0);
    train_set.insert(train_set.end(), class_1.begin(), class_1.begin() + train_size_1);

    test_set.insert(test_set.end(), class_0.begin() + train_size_0, class_0.end());
    test_set.insert(test_set.end(), class_1.begin() + train_size_1, class_1.end());

    std::shuffle(train_set.begin(), train_set.end(), g);
    std::shuffle(test_set.begin(), test_set.end(), g);

}

inline void RunTests(const Perceptron& model, const MutableArraySequence<TrainingSample>& test_set) {
    model.Evaluate(test_set);
}

#endif /* TEST_HPP */