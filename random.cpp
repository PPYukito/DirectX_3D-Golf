#include "random.h"


#include <iostream>
#include <random>

int GetRandomInteger(int min, int max)
{
	// 1. Obtain a random seed from the hardware
	std::random_device rd;

	// 2. Initialize the Mersenne Twister engine with the seed
	std::mt19937 gen(rd());

	// 3. Define the range [1, 100] inclusive
	std::uniform_int_distribution<int> distrib(min, max);

	return distrib(gen);
}

float GetRandomFloat(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float>distrib(min, max);

	return distrib(gen);
}
