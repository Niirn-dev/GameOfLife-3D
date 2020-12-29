#pragma once

#include "PhongSphere.h"

class TestSphere
{
public:
	TestSphere( Graphics& gfx,int nSubdivisions = 2,float radius = 1.0f );
	void Update( float dt ) noexcept;
	void Draw( Graphics& gfx ) noexcept( !IS_DEBUG );

	void SpawnControlWindow() noexcept;
private:
	void ResetPosition() noexcept;
private:
	Graphics& gfx;
	static constexpr float dAngle = 1.0f;
	int nSubdiv;
	float r;
	PhongSphere mesh;
};