#pragma once
#include "Planet.h"
#include "PointLight.h"

class Star
{
public:
	Star( Graphics& gfx,
		  const DirectX::XMFLOAT3& modelColor,
		  const DirectX::XMFLOAT3& diffuseColor,
		  int nSubdivisions,float radius,
		  unsigned short nPlanets );

	void AddPlanet( Graphics& gfx ) noexcept( !IS_DEBUG );
	void Update( float dt ) noexcept;
	void BindLightAndDraw( Graphics& gfx ) noexcept( !IS_DEBUG );

	void SpawnLightControlWindow() noexcept;

private:
	std::unique_ptr<PointLight> pPointLight;

	std::vector<std::unique_ptr<Planet>> planetPtrs;
};