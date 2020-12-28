#pragma once

#include "Drawable.h"

class TestSphere : public Drawable
{
public:
	TestSphere( Graphics& gfx,int nSubdivisions = 2,float size = 1.0f );
	void Update( float dt ) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void SpawnControlWindow() noexcept;
	void BindLight( Graphics& gfx ) noexcept;
private:
	void UpdateMesh() noexcept;
	void ResetPosition() noexcept;
private:
	struct LightBuffer
	{
		alignas( 16 ) DirectX::XMFLOAT3 lightPos = { 1.0f,1.0f,0.0f };
		alignas( 16 ) DirectX::XMFLOAT3 ambient = { 0.012f,0.0f,0.008f };
		alignas( 16 ) DirectX::XMFLOAT3 diffuseColor = { 1.0f,0.0f,0.0f };
		float diffuseIntensity = 1.0f;
		float attConst = 1.0f;
		float attLin = 0.045f;
		float attQuad = 0.0075f;
	} lightCBuf;

	Graphics& gfx;
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	static constexpr float dAngle = 1.0f;
	int nSubdivisions = 2;
};