#pragma once

#include "Drawable.h"

class TestTriangle : public Drawable
{
public:
	TestTriangle( Graphics& gfx );
	void Update( DirectX::XMFLOAT3 dPos ) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = {};
};