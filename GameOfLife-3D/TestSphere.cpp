#include "TestSphere.h"
#include "BindableCommon.h"
#include "Sphere.h"
#include "imgui/imgui.h"

TestSphere::TestSphere( Graphics& gfx )
{
	const auto mesh = Sphere::MakeIcoSphere( 2 );

	AddBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_unique<VertexBuffer>( gfx,mesh.vertices ) );

	auto pvs = std::make_unique<VertexShader>( gfx,L"VertexShader.cso" );
	auto pvsb = pvs->GetBlob();
	AddBind( std::move( pvs ) );

	std::vector<D3D11_INPUT_ELEMENT_DESC> ieDesc = {
		{ "Position",0u,DXGI_FORMAT_R32G32B32_FLOAT,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0u }
	};
	AddBind( std::make_unique<InputLayout>( gfx,pvsb,mesh.vertices.GetLayout().GetD3DLayout() ) );

	AddBind( std::make_unique<PixelShader>( gfx,L"PixelShader.cso" ) );

	AddBind( std::make_unique<IndexBuffer>( gfx,mesh.indices ) );

	AddBind( std::make_unique<TransformCBuf>( gfx,*this ) );
}

void TestSphere::Update( float dt ) noexcept
{
	pitch += dAngle * dt;
	yaw += dAngle * dt;
	roll += dAngle * dt;
}

DirectX::XMMATRIX TestSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		DirectX::XMMatrixTranslation( pos.x,pos.y,pos.z );
}

void TestSphere::SpawnControlWindow() noexcept
{
	if ( ImGui::Begin( "Sphere" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&pos.x,-10.0f,10.0f,"%.1f" );
		ImGui::SliderFloat( "Y",&pos.y,-10.0f,10.0f,"%.1f" );
		ImGui::SliderFloat( "Z",&pos.z,-10.0f,10.0f,"%.1f" );

		if ( ImGui::Button( "Reset" ) )
		{
			Reset();
		}

		ImGui::End();
	}
}

void TestSphere::Reset() noexcept
{
	pos = { 0.0f,0.0f,0.0f };
}
