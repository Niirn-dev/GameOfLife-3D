#pragma once

#include "PhongSphere.h"
#include <random>

class TestSphere
{
public:
	TestSphere( Graphics& gfx,
				std::mt19937& rng,
				std::uniform_int_distribution<int>& sdDist,
				std::uniform_real_distribution<float>& rDist,
				std::normal_distribution<float>& orbitDist,
				std::uniform_real_distribution<float>& aDist,
				std::uniform_real_distribution<float>& asDist );
	void Update( float dt ) noexcept;
	void Draw( Graphics& gfx ) noexcept( !IS_DEBUG );
private:
	PhongSphere mesh;
	const float dAngle = 1.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};