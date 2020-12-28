#pragma once

#include "Drawable.h"

class TestSphere : public Drawable
{
public:
	TestSphere( Graphics& gfx,int nSubdivisions = 2,float size = 1.0f );
	void Update( float dt ) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow() noexcept;
	void SetPosition( DirectX::XMFLOAT3 pos_in ) noexcept;
private:
	void UpdateMesh() noexcept;
	void ResetPosition() noexcept;
private:
	Graphics& gfx;
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	static constexpr float dAngle = 1.0f;
	int nSubdivisions = 2;
};