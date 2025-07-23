// ReinforcementGames.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>

constexpr int maxCars = 20;
int carsA = 10;
int carsB = 10;
int state[maxCars + 1][maxCars + 1]; //additional state for no cars, so 20 cars max + 1 state with no cars
int policy[maxCars + 1][maxCars + 1] = { 0 };
constexpr double discountFactor = 0.9;
constexpr double reward = 10;
constexpr double moveCost = 2;
constexpr double convergence_epsilon = 0.5;
constexpr double EULER_E = 2.71828182845904523536;

double Poisson(int n, double lambda)
{
	double probability = std::pow(EULER_E, -lambda) * std::pow(lambda, n) / std::tgamma(n + 1);
	return probability;
}

int main()
{
	bool converged = false;

	while (!converged)
	{
		converged = true;

		for (int carsA = 0; carsA <= maxCars; ++carsA)
		{
			for (int carsB = 0; carsB <= maxCars; ++carsB)
			{
				//1. Action move cars
				int k = policy[carsA][carsB];
				int movedA = std::max(0, std::min(maxCars, carsA - k));
				int movedB = std::max(0, std::min(maxCars, carsB + k));

				double newValue = 0.0;

				for (int rentA = 0; rentA <= 10; rentA++)
				{
					for (int returnA = 0; returnA <= 10; returnA++)
					{
						for (int rentB = 0; rentB <= 10; rentB++)
						{
							for (int returnB = 0; returnB <= 10; returnB++)
							{
								// 1. Compute joint probability

								double probRentA = Poisson(rentA, 3);
								double probRentB = Poisson(rentB, 4);
								double probReturnA = Poisson(returnA, 3);
								double probReturnB = Poisson(returnB, 2);
								double totalProb = probRentA * probReturnA * probRentB * probReturnB;

								// 2. Compute actual rentals
								int carsRentedA = std::min(movedA, rentA);
								int carsRentedB = std::min(movedB, rentB);

								// 3. Compute next state

								int afterReturnCarsA = std::min(movedA - carsRentedA + returnA, maxCars);
								int afterReturnCarsB = std::min(movedB - carsRentedB + returnB, maxCars);


								// 4. Compute reward

								int rewardA = carsRentedA * 10;
								int rewardB = carsRentedB * 10;
								int movementCost = 2 * std::abs(k);

								// 5. Accumulate expected value
								newValue += totalProb * (rewardA + rewardB - movementCost + discountFactor * state[afterReturnCarsA][afterReturnCarsB]);
							}
						}
					}
				}

				double delta = std::fabs(state[carsA][carsB] - newValue);
				if (delta > convergence_epsilon)
					converged = false;

				state[carsA][carsB] = newValue;
			}
		}
	}

	for (int action = -5; action <= 5; action++)
	{

	}

	return 0;
}
