#pragma once

#include "PhongSphere.h"
#include <random>
#include "NiiMath.h"

class Planet
{
private:
	class Factory
	{
	private:
		Factory() noexcept;
	public:
		static Factory& Get() noexcept;

		std::unique_ptr<Planet> PlanetPtr( Graphics& gfx,int level ) const noexcept;
		int Subdivision( int level ) noexcept;
		float Radius( int level ) noexcept;
		float Orbit( int level ) noexcept;
		float Angle( int level ) noexcept;
		float AngleSpeed( int level ) noexcept;
		int Moons( int level ) noexcept;

	private:
		static Factory fctry;

		static constexpr int minSubdiv = 0;
		static constexpr int maxSubdiv = 4;
		static constexpr float meanRadius = 8.0f;
		static constexpr float sigmaRadius = 3.0f;
		static constexpr int minOrbit = 0;
		static constexpr int maxOrbit = 6;
		static constexpr float minAngle = -PI;
		static constexpr float maxAngle = PI;
		static constexpr float minAngleSpeed = -PI / 16.0f;
		static constexpr float maxAngleSpeed = PI / 16.0f;
		static constexpr int minMoons = 0;
		static constexpr int maxMoons = 3;

		std::mt19937 rng;
		std::uniform_int_distribution<int> subdivDist;
		std::normal_distribution<float> rDist;
		std::uniform_int_distribution<int> nOrbitDist;
		std::uniform_real_distribution<float> angleDist;
		std::uniform_real_distribution<float> angleSpeedDist;
		std::uniform_int_distribution<int> nMoonsDist;
	};
public:
	Planet( Graphics& gfx,int level );
	void Update( float dt ) noexcept;
	void Draw( Graphics& gfx ) noexcept( !IS_DEBUG );
private:
	void UpdatePosition( float dt ) noexcept;
	void WrapAngles() noexcept;

private:
	const DirectX::XMFLOAT3 startPos = {};
	static constexpr float minRadius = 0.3f;
	const float radius = 0.4f;
	const float dPlanetPitch = 1.0f;
	const float dPlanetYaw = 1.0f;
	const float dPlanetRoll = 1.0f;
	const float dPitch = 1.0f;
	const float dYaw = 1.0f;
	const float dRoll = 1.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	PhongSphere mesh;
	std::vector<std::unique_ptr<Planet>> moonPtrs;
};