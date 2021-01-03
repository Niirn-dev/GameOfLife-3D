#pragma once
#include "Planet.h"

class Star
{
public:
	Star( Graphics& gfx,
		  std::mt19937& rng,
		  std::uniform_int_distribution<int> subdivDist,
		  std::normal_distribution<float> rDist,
		  std::uniform_int_distribution<unsigned int> nOrbitDist,
		  std::uniform_real_distribution<float> angleDist,
		  std::uniform_real_distribution<float> angleVelDist,
		  std::uniform_int_distribution<unsigned int> nMoonsDist );
private:
	std::vector<std::unique_ptr<Planet>> planetPtrs;
};