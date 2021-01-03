#include "Camera.h"
#include "imgui\imgui.h"

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

void Camera::SpawnControlWindow() noexcept
{
    if ( ImGui::Begin( "Camera" ) )
    {
        ImGui::Text( "Position" );
        ImGui::SliderFloat( "X",&pos.x,-70.0f,70.0f,"%.1f" );
        ImGui::SliderFloat( "Y",&pos.y,-70.0f,70.0f,"%.1f" );
        ImGui::SliderFloat( "Z",&pos.z,-70.0f,70.0f,"%.1f" );

        ImGui::Text( "Orientation" );
        ImGui::SliderAngle( "Pitch",&pitch,-89.95f,89.95f );
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
    dx::XMStoreFloat3( &pos,{ 0.0f,0.0f,-70.0f } );
    pitch = 0.0f;
    yaw = 0.0f;
}
