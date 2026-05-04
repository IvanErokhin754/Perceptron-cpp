#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <vector>
#include <string>
#include "Perceptron.hpp"
#include "TestData.hpp"

inline const char* Label(int value) {
    return value == 1 ? "GO" : "REST";
}

void TestModel(const Perceptron& model, const TestSample& test) {
    int prediction = model.Predict(test.input.input);

    std::cout << "TEST: " << test.description << '\n';
    std::cout << "EXPECTED: " << Label(test.expected) << '\n';
    std::cout << "RESULT: " << Label(prediction) << '\n';

    if (prediction == test.expected) {
        std::cout << "STATUS: OK\n";
    } else {
        std::cout << "STATUS: WRONG\n";
    }

    std::cout << "--------------------------------\n";
}

void RunTests(const Perceptron& model) {
    std::vector<TestSample> tests = GetTestSamples();

    int correct = 0;

    for (const TestSample& test : tests) {
        int prediction = model.Predict(test.input.input);

        std::cout << "TEST: " << test.description << '\n';
        std::cout << "EXPECTED: " << Label(test.expected) << '\n';
        std::cout << "RESULT: " << Label(prediction) << '\n';

        if (prediction == test.expected) {
            std::cout << "STATUS: OK\n";
            correct++;
        } else {
            std::cout << "STATUS: WRONG\n";
        }

        std::cout << "--------------------------------\n";
    }

    std::cout << "TOTAL: " << correct << "/" << tests.size() << " correct\n";
}

#endif