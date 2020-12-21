#include "Camera.h"

namespace dx = DirectX;

Camera::Camera() noexcept
{
    Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    const auto vecPos = dx::XMLoadFloat3( &pos );
    const auto vecLookAt = dx::XMVectorAdd(
        vecPos,
        dx::XMVector3Transform(
            dx::XMVectorSet( 0.0f,0.0f,1.0f,0.0f ),
            dx::XMMatrixRotationRollPitchYaw( pitch,yaw,0.0f )
        )
    );

    return DirectX::XMMatrixLookAtLH(
        vecPos,
        vecLookAt,
        dx::XMVectorSet( 0.0f,1.0f,0.0f,0.0f )
    );
}

void Camera::Reset() noexcept
{
    dx::XMStoreFloat3( &pos,{ 0.0f,0.0f,-10.0f } );
    pitch = 0.0f;
    yaw = 0.0f;
}
