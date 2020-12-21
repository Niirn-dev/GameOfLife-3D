#pragma once

#include "Drawable.h"

class TestSphere : public Drawable
{
public:
	TestSphere( Graphics& gfx );
	void Update( float dt ) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,10.0f };
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	static constexpr float dAngle = 1.0f;
};