#pragma once

#include "PhongSphere.h"
#include <random>
#include "NiiMath.h"

class Planet
{
private:
	struct Angles
	{
		float pitch = 0.0f;
		float yaw = 0.0f;
		float roll = 0.0f;
	};
	struct Attributes
	{
		float radius = 0.0f;
		Angles angle;
		Angles angleSpeed;
	};
	struct OrbitAttributes
	{
		unsigned short index = 0u;
		Attributes attr;
	};

	class Factory
	{
	private:
		Factory() noexcept;
		static Factory& Get() noexcept;
	public:
		static std::unique_ptr<Planet> PlanetPtr( Graphics& gfx,int level ) noexcept;

		static OrbitAttributes OrbitAttr( int level ) noexcept;
		static Attributes PlanetAttr( int level,int orbitIndex ) noexcept;
		static int MoonCount( int level,int orbit ) noexcept;
		static int Subdivision( int level ) noexcept;

	private:
		int OrbitIndex( int level ) noexcept;
		float PlanetRadius( int level,int orbit ) noexcept;
		float OrbitRadius( int level,int orbit ) noexcept;
		Angles GenerateAngles() noexcept;
		Angles GenerateAngleSpeeds() noexcept;

	private:
		static Factory fctry;

		static constexpr int minSubdiv = 0;
		static constexpr int maxSubdiv = 4;
		static constexpr float meanRadius = 3.0f;
		static constexpr float sigmaRadius = 1.5f;
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
	void Draw( Graphics& gfx, DirectX::FXMMATRIX parentTransform = DirectX::XMMatrixIdentity() ) noexcept( !IS_DEBUG );
private:
	void UpdatePosition( float dt ) noexcept;
	void WrapAngles() noexcept;

private:
	OrbitAttributes orbitAttrs;
	Attributes planetAttrs;

	std::unique_ptr<PhongSphere> pMesh;
	std::vector<std::unique_ptr<Planet>> moonPtrs;
};