#pragma once

#include "Drawable.h"

class TestSphere : public Drawable
{
public:
	TestSphere( Graphics& gfx );
	void Update( DirectX::XMFLOAT3 dPos ) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,2.0f };
};