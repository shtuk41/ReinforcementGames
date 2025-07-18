// ReinforcementGames.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>

constexpr int width = 4;
constexpr int height = 4;
constexpr int numberSweep = 10;
constexpr int numberStates = width * height;
constexpr int numberActions = 4;
constexpr double discountFactor = 1;
constexpr double reward = -1;

enum class action : uint8_t
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

void improvePolicy(double *p, double *v,int up, int down, int left, int right)
{
    double stateValues[4] = { v[up], v[down], v[left], v[right] };

    auto max_it = std::max_element(stateValues, stateValues + 4);
    int element = (int)std::distance(stateValues, max_it);

    for (int ii = 0; ii < 4; ii++)
    {
        p[ii] = ii == element ? 1.0 : 0.0;
    }
}

int main()
{
    double V[numberStates] = { 0.0 };
    double PI[numberStates][4];

    std::fill_n(&PI[0][0], numberStates * 4, 0.25);
    
    for (int ii = 0; ii < numberSweep; ii++)
    {
        V[1] = PI[1][static_cast<int>(action::UP)] * (-1 + discountFactor * V[1]) +
            PI[1][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[5]) +
            PI[1][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[0]) +
            PI[1][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[2]);

        V[2] = PI[2][static_cast<int>(action::UP)] * (-1 + discountFactor * V[2]) +
            PI[2][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[6]) +
            PI[2][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[1]) +
            PI[2][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[3]);

        V[3] = PI[3][static_cast<int>(action::UP)] * (-1 + discountFactor * V[3]) +
            PI[3][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[7]) +
            PI[3][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[2]) + 
            PI[3][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[3]);

        V[4] = PI[4][static_cast<int>(action::UP)] * (-1 + discountFactor * V[0]) +
            PI[4][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[8]) +
            PI[4][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[4]) + 
            PI[4][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[5]);

        V[5] = PI[5][static_cast<int>(action::UP)] * (-1 + discountFactor * V[1]) +
            PI[5][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[9]) + 
            PI[5][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[4]) +
            PI[5][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[6]);

        V[6] = PI[6][static_cast<int>(action::UP)] * (-1 + discountFactor * V[2]) +
            PI[6][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[10]) + 
            PI[6][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[5]) +
            PI[6][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[7]);

        V[7] = PI[7][static_cast<int>(action::UP)] * (-1 + discountFactor * V[3]) +
            PI[7][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[11]) + 
            PI[7][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[6]) +
            PI[7][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[7]);

        V[8] = PI[8][static_cast<int>(action::UP)] * (-1 + discountFactor * V[4]) +
                PI[8][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[12]) +
                PI[8][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[8]) +
            PI[8][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[9]);

        V[9] = PI[9][static_cast<int>(action::UP)] * (-1 + discountFactor * V[5]) +
                PI[9][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[13]) +
                PI[9][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[8]) +
            PI[9][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[10]);

        V[10] = PI[10][static_cast<int>(action::UP)] * (-1 + discountFactor * V[6]) +
                PI[10][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[14]) +
                PI[10][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[9]) +
            PI[10][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[11]);

        V[11] = PI[11][static_cast<int>(action::UP)] * (-1 + discountFactor * V[7]) +
                PI[11][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[15]) +
                PI[11][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[10]) +
            PI[11][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[11]);

        V[12] = PI[12][static_cast<int>(action::UP)] * (-1 + discountFactor * V[8]) +
                PI[12][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[12]) +
                PI[12][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[12]) +
            PI[12][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[13]);

        V[13] = PI[13][static_cast<int>(action::UP)] * (-1 + discountFactor * V[9]) +
                PI[13][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[13]) +
                PI[13][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[12]) +
            PI[13][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[14]);

        V[14] = PI[14][static_cast<int>(action::UP)] * (-1 + discountFactor * V[10]) +
                PI[14][static_cast<int>(action::DOWN)] * (-1 + discountFactor * V[14]) +
                PI[14][static_cast<int>(action::LEFT)] * (-1 + discountFactor * V[13]) +
            PI[14][static_cast<int>(action::RIGHT)] * (-1 + discountFactor * V[15]);
    }

    improvePolicy(PI[0], V, 0,4,0,1);
    improvePolicy(PI[1], V, 1,5,0,2);
    improvePolicy(PI[2], V, 2,6,1,3);
    improvePolicy(PI[3], V, 3,7,2,3);
    improvePolicy(PI[4], V, 0,8,4,5);
    improvePolicy(PI[5], V, 1,9,4,6);
    improvePolicy(PI[6], V, 2,10,5,7);
    improvePolicy(PI[7], V, 3,11,6,7);
    improvePolicy(PI[8], V, 4,12,8,9);
    improvePolicy(PI[9], V, 5,13,8,10);
    improvePolicy(PI[10], V, 6,14,9,11);
    improvePolicy(PI[11], V, 7,15,10,11);
    improvePolicy(PI[12], V, 8,12,12,13);
    improvePolicy(PI[13], V, 9,13,12,14);
    improvePolicy(PI[14], V, 10,14,13,15);
    improvePolicy(PI[15], V, 11,15,14,15);

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
