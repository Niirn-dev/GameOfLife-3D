#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	Camera() noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
private:
	void Reset() noexcept;
private:
	DirectX::XMFLOAT3 pos = {};
	float pitch = 0.0f;
	float yaw = 0.0f;
	static constexpr float travelSpeed = 10.0f;
	static constexpr float rotationSpeed = 0.1f;
};