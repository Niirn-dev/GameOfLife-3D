#pragma once

#include "Drawable.h"

class PhongSphere : public Drawable
{
public:
	PhongSphere( Graphics& gfx,int nSubdivisions = 2,float radius = 1.0f );
	void SetSecondaryTransformation( DirectX::FXMMATRIX transform ) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	
	const DirectX::XMFLOAT3& GetPosition() const noexcept;
	void SetPosition( const DirectX::XMFLOAT3& pos_in ) noexcept;
	void SetPosition( DirectX::FXMVECTOR pos_in ) noexcept;
	void IncPosition( const DirectX::XMFLOAT3& dPos ) noexcept;
	void SetOrientation( float pitch_in,float yaw_in,float roll_in ) noexcept;
	void IncOrientation( float dPitch,float dYaw,float dRoll ) noexcept;
	void SetRadius( float radius ) noexcept;
	void SetMeshSubdivisions( Graphics& gfx,int nSubdivisions ) noexcept;
private:
	void WrapAngles() noexcept;
private:
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	static constexpr float dAngle = 1.0f;
	float r;
	DirectX::XMMATRIX secondaryTransform = DirectX::XMMatrixIdentity();
};