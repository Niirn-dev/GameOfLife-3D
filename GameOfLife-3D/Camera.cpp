#include "Camera.h"
#include "imgui\imgui.h"

namespace dx = DirectX;

Camera::Camera() noexcept
{
    Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
    const auto rotMtx = dx::XMMatrixRotationRollPitchYaw( pitch,yaw,0.0f );
    const auto vecPos = dx::XMVector3Transform(
        dx::XMVectorSet( 0.0f,0.0f,-r,0.0f ),
        rotMtx
    );
    const auto vecUp = dx::XMVector3Transform(
            dx::XMVectorSet( 0.0f,1.0f,0.0f,0.0f ),
            rotMtx
    );

    return DirectX::XMMatrixLookAtLH(
        vecPos,
        dx::XMVectorSet( 0.0f,0.0f,0.0f,0.0f ),
        vecUp
    );
}

void Camera::SpawnControlWindow() noexcept
{
    if ( ImGui::Begin( "Camera" ) )
    {
        ImGui::SliderFloat( "Distance",&r,1.0f,70.0f,"%.1f" );
        ImGui::SliderAngle( "Pitch",&pitch,-180.0f,180.0f );
        ImGui::SliderAngle( "Yaw",&yaw,-180.0f,180.0f );

        if ( ImGui::Button( "Reset" ) )
        {
            Reset();
        }

        ImGui::End();
    }
}

void Camera::Reset() noexcept
{
    r = 70.0f;
    pitch = 0.0f;
    yaw = 0.0f;
}
