#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include <iostream>

#include <cstddef>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <random>
#include "MutableArraySequence.hpp"
#include <initializer_list>

#include "Matrix.hpp"

const double eps = 1e-9;

struct TrainingSample {
    MutableArraySequence<double> input;
    int target; 
    TrainingSample() : target(0) {}

    TrainingSample(std::initializer_list<double> list, int t) : target(t) {
        for (double value : list) {
            input.Append(value);
        }
    }
    
};

class Perceptron{
private:
    double b; // смещение
    double learning_rate; // скорость обучения
    MutableArraySequence<double> weights; // вектор весов 

    MutableArraySequence<double> mean;
    MutableArraySequence<double> std_dev;

    double Sigmoid(double z) const {
        return 1.0 / (1.0 + std::exp(-z));
    }

public:    
    Perceptron(size_t input_size, double lr = 0.1) : b(0.0), learning_rate(lr), weights(input_size, 0.0) {}

    int Predict(const MutableArraySequence<double>& input) const {
        MutableArraySequence<double> x_norm = Transform(input);
        double probability = Forward(x_norm);

        return probability >= 0.5 ? 1 : 0;
    }

    double Forward(const MutableArraySequence<double>& input) const {
        if (input.size() != weights.size())
            throw std::invalid_argument("Input sizes do not match weights size");

        double z = b;
        for (size_t i = 0; i < input.size(); i++) {
            z += weights[i] * input[i];
        }

        return Sigmoid(z); // посчитанную взвешенную сумму пропускаем через сигмоиду
    }

    void TrainBatch(const Matrix<double>& X, const MutableArraySequence<double>& y) {
        size_t m = X.GetRows(); // размер батча
        if (m == 0) return;

        MutableArraySequence<double> z = X.MultiplyByVector(weights);
        MutableArraySequence<double> y_hat(m);

        for (size_t i = 0; i < m; i++) {
            y_hat[i] = Sigmoid(z[i] + b);
        }

        MutableArraySequence<double> diff(m);
        double bias_grad = 0.0;
        for (size_t i = 0; i < m; i++) {
            diff[i] = y_hat[i] - y[i];
            bias_grad += diff[i]; 
        }
        bias_grad /= m; 

        MutableArraySequence<double> weights_grad = X.TransposeMultiplyByVector(diff);

        for (size_t j = 0; j < weights.size(); j++) {
            weights[j] -= learning_rate * (weights_grad[j] / m); 
        }
        b -= learning_rate * bias_grad;
    }

    double ComputeLoss(const MutableArraySequence<TrainingSample>& X) const {
        double loss = 0.0;
        size_t m = X.size();
        for (size_t i = 0; i < m; i++) {
            double y_hat = Forward(X[i].input);
            double y = X[i].target;

            double eps_log = 1e-15;
            double clipped_y_hat = std::max(eps_log, std::min(1.0 - eps_log, y_hat));

            loss += y * std::log(clipped_y_hat) + (1.0 - y) * std::log(1.0 - clipped_y_hat);
        }
        
        return -loss / m;
    }

    void Train(MutableArraySequence<TrainingSample> X, int epochs, size_t batch_size = 32) {
        if (epochs <= 0)
            throw std::invalid_argument("Epochs must be positive");

        FitScaler(X);

        for (size_t i = 0; i < X.size(); i++) {
            X[i].input = Transform(X[i].input);
        }

        std::random_device rd;
        std::mt19937 g(rd());
        
        size_t current_batch_size;

        for (int epoch = 0; epoch < epochs; epoch++) {
            std::shuffle(X.begin(), X.end(), g);
            for (size_t i = 0; i < X.size(); i += batch_size) {
                current_batch_size = std::min(batch_size, X.size() - i);

                Matrix<double> X_batch(current_batch_size, weights.size());
                MutableArraySequence<double> y_batch(current_batch_size);

                for (size_t j = 0; j < current_batch_size; j++) {
                    for (size_t k = 0; k < weights.size(); k++) {
                        X_batch.Set(j, k, X[i + j].input[k]); 
                    }

                    y_batch[j] = X[i + j].target; 
                }   

                TrainBatch(X_batch, y_batch);
            }

            double loss = ComputeLoss(X);

            std::cout << "Epoch: " << epoch + 1 << ", errors: " << loss << std::endl;

            if (loss < 1e-4) {
                std::cout << "Training stopped: loss is close to zero\n";
                break;
            }
        }
    }
    
    void Evaluate(const MutableArraySequence<TrainingSample>& test_data) const {
        int TP = 0;
        int TN = 0;
        int FP = 0;
        int FN = 0;
        
        for (size_t i = 0; i < test_data.size(); i++) {
            int prediction = Predict(test_data[i].input);
            int actual = test_data[i].target;

            if (prediction == 1 && actual == 1) TP++;
            else if (prediction == 1 && actual == 0)  FP++;
            else if (prediction == 0 && actual == 1) FN++;
            else if (prediction == 0 && actual == 0) TN++;
        }

        double accuracy = (TP + TN + FP + FN) == 0 ? 0.0 : static_cast<double>(TP + TN) / (TP + TN + FP + FN);
        double precision = (TP + FP) == 0 ? 0.0 : static_cast<double>(TP) / (TP + FP);
        double recall = (TP + FN) == 0 ? 0.0 : static_cast<double>(TP) / (TP + FN);
        double f1_score = (precision + recall) == 0 ? 0.0 : 2.0 * (precision * recall) / (precision + recall);

        std::cout << "\n-----EVALUATE METRICS-----\n";
        std::cout << "Accuracy: " << accuracy << std::endl;
        std::cout << "Precision: " << precision << std::endl;
        std::cout << "Recall: " << recall << std::endl;
        std::cout << "F1-score: " << f1_score << std::endl;
        std::cout << "-----------------------------\n";
    }

    void FitScaler(const MutableArraySequence<TrainingSample>& X) {
        size_t d = weights.size();
        size_t m = X.size();
        mean.assign(d, 0.0);
        std_dev.assign(d, 0.0);

        if (m == 0)
            return;
        
        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < d; j++) {
                mean[j] += X[i].input[j];
            }
        }

        for (size_t j = 0; j < d; j++) {
            mean[j] /= m;
        }

        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < d; j++) {
                double diff = X[i].input[j] - mean[j];
                std_dev[j] += diff * diff;
            }
        }

        for (size_t j = 0; j < d; j++) {
            std_dev[j] = std::sqrt(std_dev[j] / m);
        }
    }

    MutableArraySequence<double> Transform(const MutableArraySequence<double>& x) const {
        MutableArraySequence<double> x_norm = x;
        if (mean.empty() || std_dev.empty()) 
            return x_norm;
        
        for (size_t i = 0; i < x.size(); i++) {
            if (std_dev[i] > eps)
                x_norm[i] = (x[i] - mean[i]) / std_dev[i];
            else
                x_norm[i] = x[i] - mean[i];
        }

        return x_norm;
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


