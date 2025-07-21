// ReinforcementGames.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>

constexpr int width = 4;
constexpr int height = 4;
constexpr int numberSweep = 1000000;
constexpr int numberStates = width * height;
constexpr int numberActions = 4;
constexpr double discountFactor = 1;
constexpr double reward = -1;
constexpr double convergence_epsilon = 0.5;

enum class action : uint8_t
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

void improvePolicy(int num, double p[][4], double* v, bool& stable,  int up, int down, int left, int right)
{
    auto max_it_pi = std::max_element(p[num], p[num] + 4);
    int prev_action = (int)std::distance(p[num], max_it_pi);


    double stateValues[4] = { v[up], v[down], v[left], v[right] };

    auto max_it = std::max_element(stateValues, stateValues + 4);
    int best_action = (int)std::distance(stateValues, max_it);

    if (prev_action != best_action)
        stable = false;

    for (int ii = 0; ii < 4; ii++)
    {
        p[num][ii] = ii == best_action ? 1.0 : 0.0;
    }
}

void converge(int num, double* v, double p[][4], bool &converged, int up, int down, int left, int right)
{
    double v_old = v[num];

    v[num] = p[num][static_cast<int>(action::UP)] * (-1 + discountFactor * v[up]) +
        p[num][static_cast<int>(action::DOWN)] * (-1 + discountFactor * v[down]) +
        p[num][static_cast<int>(action::LEFT)] * (-1 + discountFactor * v[left]) +
        p[num][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * v[right]);

    if (std::fabs(v[num] - v_old) > convergence_epsilon)
        converged = false;
}

int main()
{
    double V[numberStates] = { 0.0 };
    double PI[numberStates][4];

    std::fill_n(&PI[0][0], numberStates * 4, 0.25);
    
    for (int ii = 0; ii < numberSweep; ii++)
    {
        bool converged = false;

        while (!converged)
        {
            converged = true;

            converge(1, V, PI, converged, 1, 5, 0, 2);
            converge(2, V, PI, converged, 2, 6, 1, 3);
            converge(3, V, PI, converged, 3, 7, 2, 3);
            converge(4, V, PI, converged, 0, 8, 4, 5);
            converge(5, V, PI, converged, 1, 9, 4, 6);
            converge(6, V, PI, converged, 2, 10, 5, 7);
            converge(7, V, PI, converged, 3, 11, 6, 7);
            converge(8, V, PI, converged, 4, 12, 8, 9);
            converge(9, V, PI, converged, 5, 13, 8, 10);
            converge(10, V, PI, converged, 6, 14, 9, 11);
            converge(11, V, PI, converged, 7, 15, 10, 11);
            converge(12, V, PI, converged, 8, 12, 12, 13);
            converge(13, V, PI, converged, 9, 13, 12, 14);
            converge(14, V, PI, converged, 10, 14, 13, 15);
        }

        bool stable = true;

        improvePolicy(0, PI, V, stable, 0, 4, 0, 1);
        improvePolicy(1, PI, V, stable, 1, 5, 0, 2);
        improvePolicy(2, PI, V, stable, 2, 6, 1, 3);
        improvePolicy(3, PI, V, stable, 3, 7, 2, 3);
        improvePolicy(4, PI, V, stable, 0, 8, 4, 5);
        improvePolicy(5, PI, V, stable, 1, 9, 4, 6);
        improvePolicy(6, PI, V, stable, 2, 10, 5, 7);
        improvePolicy(7, PI, V, stable, 3, 11, 6, 7);
        improvePolicy(8, PI, V, stable, 4, 12, 8, 9);
        improvePolicy(9, PI, V, stable, 5, 13, 8, 10);
        improvePolicy(10,PI, V, stable, 6, 14, 9, 11);
        improvePolicy(11,PI, V, stable, 7, 15, 10, 11);
        improvePolicy(12,PI, V, stable, 8, 12, 12, 13);
        improvePolicy(13,PI, V, stable, 9, 13, 12, 14);
        improvePolicy(14,PI, V, stable, 10, 14, 13, 15);
        improvePolicy(15,PI, V, stable, 11, 15, 14, 15);

        if (stable)
            break;
    }

    for (int jj = 0; jj < height; jj++)
    {
        for (int ii = 0; ii < width; ii++)
        {
            int index = jj * width + ii;
            double stateValue = V[index];

            char ba = '@';

            for (int a = 0; a < 4; a++)
            {
                if (PI[index][a] > 0)
                {
                    if (a == 0)
                        ba = '^';
                    else if (a == 1)
                        ba = 'v';
                    else if (a == 2)
                        ba = '<';
                    else if (a == 3)
                        ba = '>';
                }
            }

            std::cout << std::fixed << std::setprecision(1) << stateValue << ba << ',';

            if (ii == width - 1)
                std::cout << '\n';
        }
    }
}
