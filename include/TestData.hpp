#ifndef TEST_DATA_HPP
#define TEST_DATA_HPP

#include <vector>
#include "Perceptron.hpp"

struct TestSample {
    TrainingSample input;
    int expected;
    std::string description;
};


std::vector<TestSample> tests = {
    {{{1,0,0, 1,0,0, 0, 1, 1, 0, 0,1,0, 1}, 0}, 0,
    "Мало сна + плохое состояние + ел/пил"
    },

    {{{0,1,0, 0,1,0, 1, 1, 1, 1, 0,1,0, 1}, 0}, 0,
    "Вчера тяжело + крепатура"
    },

    {{{0,1,0, 0,1,0, 0, 1, 0, 0, 0,1,0, 0}, 1}, 1,
    "Нормальное состояние + ел, но не пил + нет восстановления"
    },

    {{{1,0,0, 0,0,1, 1, 1, 1, 0, 0,1,0, 1}, 1}, 0,
    "Мало сна + хорошее состояние + вчера тяжело, но без крепатуры"
    },

    {{{0,1,0, 1,0,0, 1, 1, 1, 0, 0,1,0, 1}, 0}, 0,
    "Плохое состояние + вчера тяжело"
    }

};

std::vector<TestSample> GetTestSamples() {
    return tests;
}


#endif /* TEST_DATA_HPP */
