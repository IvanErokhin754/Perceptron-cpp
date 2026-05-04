#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include <iostream>
#include <vector>
#include <cstddef>
#include <stdexcept>

struct TrainingSample {
    std::vector<int> input;
    int target;
};

class Perceptron{
private:
    double b; // смещение
    double learning_rate; // скорость обучения
    std::vector<double> weights; // вектор весов 

public:    
    Perceptron(size_t input_size, double lr = 0.1) : b(0.0), learning_rate(lr), weights(input_size, 0.0) {}

    int Predict(const std::vector<int>& input) const{
        if (input.size() != weights.size())
            throw std::invalid_argument("Input size does not match weights size");

        double z = b;

        for (size_t i = 0; i < input.size(); i++) {
            z += weights[i] * input[i];
        }

        return z >= 0 ? 1 : 0;
    }

    void UpdateWeights(const std::vector<int>& input, int error) {
        if (input.size() != weights.size())
            throw std::invalid_argument("Input size does not match weights size");

        for (size_t i = 0; i < weights.size(); i++) {
            weights[i] += learning_rate * error * input[i];
        }

        b += learning_rate * error;
    }

    bool TrainOne(const std::vector<int>& input, int target) {
        int prediction = Predict(input);
        int error = target - prediction;

        if (error != 0) {
            UpdateWeights(input, error);
            return true; // была ошибка
        }
        
        return false; // ошибки не было

    }

    void Train(const std::vector<TrainingSample>& X, int epochs) {
        if (epochs <= 0)
            throw std::invalid_argument("Epochs must be positive");

        int errors_counter;
        bool had_error;
        for (int epoch = 0; epoch < epochs; epoch++) {
            errors_counter = 0;

            for (size_t i = 0; i < X.size(); i++) {
                had_error = TrainOne(X[i].input, X[i].target);

                if (had_error) {
                    errors_counter++; 
                }
            }

            std::cout << "Epoch: " << epoch + 1 << ", errors: " << errors_counter << std::endl;

            if (errors_counter == 0) {
                std::cout << "Training stopped: no errors\n";
                break;
            }
        }
    }
    
    void PrintWeights() const {
        std::cout << "--------Weights--------\n";

        for (size_t i = 0; i < weights.size(); i++) {
            std::cout << "w[" << i << "] = " << weights[i] << ' ';
        }

        std::cout << "\nb = " << b << std::endl;
        std::cout << "\n------------------------\n";
    }

};

#endif /* PERCEPTRON_HPP */


