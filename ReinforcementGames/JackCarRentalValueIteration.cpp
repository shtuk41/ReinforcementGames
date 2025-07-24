// ReinforcementGames.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <random>
#include <string>

constexpr int maxCars = 20;
int carsA = 10;
int carsB = 10;
double state[maxCars + 1][maxCars + 1]; //additional state for no cars, so 20 cars max + 1 state with no cars
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

void InitializePoisson(std::array<double, 11>& arr, double average)
{
	for (int ii = 0; ii <= 9; ii++)
	{
		arr[ii] = Poisson(ii, average);
	}

	arr[10] = 1.0 - std::accumulate(arr.begin(), arr.begin() + 10, 0.0);
}

int main()
{
	//precompute Poisson
	std::array<double, 11> rentAProbability = { 0 };
	std::array<double, 11>  rentBProbability = { 0 };
	std::array<double, 11>  returnAProbability = { 0 };
	std::array<double, 11>  returnBProbability = { 0 };

	InitializePoisson(rentAProbability, 3);
	InitializePoisson(rentBProbability, 4);
	InitializePoisson(returnAProbability, 3);
	InitializePoisson(returnBProbability, 2);

	bool converged = false;

	while (!converged)
	{
		converged = true;

		for (int carsA = 0; carsA <= maxCars; ++carsA)
		{
			for (int carsB = 0; carsB <= maxCars; ++carsB)
			{
				double bestValue = -1e9;
				int bestAction = 0;

				int minAction = -std::min(5, carsB);
				int maxAction = std::min(5, carsA);

				for (int action = minAction; action <= maxAction; action++)
				{
					//1. Action move cars
					int movedA = std::max(0, std::min(maxCars, carsA - action));
					int movedB = std::max(0, std::min(maxCars, carsB + action));

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

									double probRentA = rentAProbability[rentA];
									double probRentB = rentBProbability[rentB];
									double probReturnA = returnAProbability[returnA];
									double probReturnB = returnBProbability[returnB];
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
									int movementCost = 2 * std::abs(action);

									// 5. Accumulate expected value
									newValue += totalProb * (rewardA + rewardB - movementCost + discountFactor * state[afterReturnCarsA][afterReturnCarsB]);
								}
							}
						}
					}
				
					if (newValue > bestValue)
					{
						bestValue = newValue;
						bestAction = action;
					}
				}

				double delta = std::fabs(state[carsA][carsB] - bestValue);
				if (delta > convergence_epsilon)
					converged = false;

				state[carsA][carsB] = bestValue;
				policy[carsA][carsB] = bestAction;
			}
		}
	}

	for (int jj = 0; jj < 11; jj++)
	{
		for (int ii = 0; ii < 11; ii++)
		{
			std::cout << policy[ii][jj] << ',';
		}

		std::cout << '\n';
	}


	return 0;
}
