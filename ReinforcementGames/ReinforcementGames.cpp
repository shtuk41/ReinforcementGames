// ReinforcementGames.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <random>
#include <string>

constexpr int width = 5;
constexpr int height = 5;
constexpr int numberEpisodes = 1000;
constexpr int numberStates = width * height;
constexpr int numberTicks = 5;
constexpr int numberActions = 4;
constexpr double discountFactor = 0.9;
constexpr double alpha = 0.25;
constexpr double epsilonThreshold = 0.1;

enum class action : uint8_t
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

int main()
{
    double Q[numberStates][4] = { 0.0 };

    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> distrStates(0, numberStates - 1); // Range: 0 to numberElements
    std::uniform_int_distribution<> distrActions(0, numberActions - 1); // Range: 0 to numberElements
    std::uniform_real_distribution<> distrEpsilone(0.0, 1.0);

    for (int ii = 0; ii < numberEpisodes; ii++)
    {
        int currentState = distrStates(gen);

        for (int jj = 0; jj < numberTicks; jj++)
        {
            int prevState = currentState;
            
            double reward = 0.0;
            double epsilon = distrEpsilone(gen);

            action nextAction;

            if (epsilon < epsilonThreshold)
                nextAction = (action)distrActions(gen);
            else
            {
                double best = Q[currentState][0];
                nextAction = static_cast<action>(0);
                for (int ii = 1; ii < numberActions; ii++)
                {
                    if (Q[currentState][ii] > best)
                    {
                        best = Q[currentState][ii];
                        nextAction = static_cast<action>(ii);
                    }
                }
            }

            if (prevState == 6)
            {
                currentState = 16;
                reward = 10.0;
            }
            else if (prevState == 8)
            {
                currentState = 18;
                reward = 5.0;
            }
            else
            {
                switch (nextAction)
                {
                case action::UP:
                    if ((currentState - width) >= 0)
                        currentState = currentState - width;
                    else
                        reward = -1.0;
                    break;
                case action::DOWN:
                    if ((currentState + width) < numberStates)
                        currentState = currentState + width;
                    else
                        reward = -1.0;
                    break;
                case action::LEFT:
                    if (currentState % width != 0)
                        currentState = currentState - 1;
                    else
                        reward = -1.0;
                    break;
                default: //case action::RIGHT
                    if ((currentState + 1) < numberStates && (currentState + 1) % width != 0)
                        currentState = currentState + 1;
                    else
                        reward = -1.0;
                    break;
                }
            }

            double maxNextQ = *std::max_element(std::begin(Q[currentState]), std::end(Q[currentState]));

            Q[prevState][int(nextAction)] = Q[prevState][int(nextAction)] +
                alpha * (reward + discountFactor * maxNextQ - Q[prevState][int(nextAction)]);
        }
    }

    for (int jj = 0; jj < height; jj++)
    {
        for (int ii = 0; ii < width; ii++)
        {
            int index = jj * width + ii;

            double best = Q[index][0];
            action bestAction = static_cast<action>(0);

            for (int kk = 1; kk < numberActions; kk++)
            {
                if (Q[index][kk] > best)
                {
                    best = Q[index][kk];
                    bestAction = static_cast<action>(kk);
                }
            }

            char ba;

            switch (bestAction)
            {
            case action::UP:
                ba = '^';
                break;
            case action::DOWN:
                ba = 'v';
                break;
            case action::LEFT:
                ba = '<';
                break;
            default://action::RIGHT
                ba = '>';
                break;
            }

            std::cout << std::fixed << std::setprecision(1) << best << ba << ',';

            if (ii == width - 1)
                std::cout << '\n';
        }
    }
}
