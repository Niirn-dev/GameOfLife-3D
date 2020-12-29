#pragma once

#include "PhongSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight( Graphics& gfx );
	void Draw( Graphics& gfx ) const noexcept( !IS_DEBUG );
	void BindLightBuffer( Graphics& gfx ) const noexcept;
	void SpawnControlWindow() noexcept;
private:
	void Reset() noexcept;
private:
	struct LightBuffer
	{
		alignas( 16 ) DirectX::XMFLOAT3 pos = { 1.0f,1.0f,-2.0f };
		alignas( 16 ) DirectX::XMFLOAT3 ambient = { 0.012f,0.0f,0.008f };
		alignas( 16 ) DirectX::XMFLOAT3 diffuse = { 1.0f,0.0f,0.0f };
		float diffuseIntensity = 1.0f;
		float attConst = 1.0f;
		float attLin = 0.045f;
		float attQuad = 0.0075f;
	} lightCBuf;
	mutable PhongSphere mesh;
	mutable std::unique_ptr<PixelConstantBuffer<LightBuffer>> pCBuff;
};
