#include <iostream>
#include "UI.hpp"
#include "Features.hpp"
#include <cstdlib>
#include <cstddef>

MutableArraySequence<double> StartUI() {
    MutableArraySequence<double> input(FEATURE_COUNT, 0);

    int sleep_time, state, mood, answer;

    std::cout << "------TRAINER------\n";
    std::cout << "Введите количество сна (0 - 24): ";
    std::cin >> sleep_time;
    if (sleep_time <= 6) {
        input[SLEEP_LOW] = 1;
    }
    else if (sleep_time < 11) {
        input[SLEEP_NORMAL] = 1;
    }
    else 
        input[SLEEP_HIGH] = 1;
    
    std::cout << "Ваше состояние (1 - разбитое, 2 - нормальное, 3 - бодрое): ";
    std::cin >> state;
    if (state == 1) {
        input[STATE_BAD] = 1;
    }
    else if (state == 2) {
        input[STATE_NORMAL] = 1;
    }
    else
        input[STATE_GOOD] = 1;

    std::cout << "Ваше настроение (1 - плохое, 2 - нормальное, 3 - хорошее): ";
    std::cin >> mood;
    if (mood == 1)
        input[MOOD_BAD] = 1;
    else if (mood == 2)
        input[MOOD_NORMAL] = 1;
    else
        input[MOOD_GOOD] = 1;


    std::cout << "Было ли питание последние несколько часов (1 - да, 2 - нет): ";
    std::cin >> answer;
    input[ATE] = (answer == 1 ? 1 : 0);

    std::cout << "Было ли питье последние несколько часов (1 - да, 2 - нет): ";
    std::cin >> answer;
    input[HYDRATED] = (answer == 1 ? 1 : 0);

    std::cout << "Есть признаки болезни (1 - да, 2 - нет): ";
    std::cin >> answer;
    if (answer == 1) {
        std::cout << "REST: болезнь - стоп-фактор\n";
        std::exit(0);
    }

    std::cout << "Есть боль в суставах / связках / острая боль (1 - да, 2 - нет): ";
    std::cin >> answer;
    if (answer == 1) {
        std::cout << "REST: боль/травма - стоп-фактор\n";
        std::exit(0);
    }

    std::cout << "Была ли вчера тяжелая тренировка (1 - да, 2 - нет): ";
    std::cin >> answer;
    input[HEAVY_YESTERDAY] = (answer == 1 ? 1 : 0);

    std::cout << "Есть сильная крепатура (1 - да, 2 - нет): ";
    std::cin >> answer;
    input[STRONG_MUSCLE_SORENESS] = (answer == 1 ? 1 : 0);

    std::cout << "После тренировки будет возможность нормально поесть, помыться, поспать/восстановиться (1 - да, 2 - нет): ";
    std::cin >> answer;
    input[TIME_FOR_RECOVERY] = (answer == 1 ? 1 : 0);

    return input;
}


