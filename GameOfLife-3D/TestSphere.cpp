#include "TestSphere.h"
#include "BindableCommon.h"
#include "Sphere.h"
#include "imgui/imgui.h"

TestSphere::TestSphere( Graphics& gfx )
	:
	gfx( gfx )
{
	const auto mesh = Sphere::MakeIcoSphere( nSubdivisions );

	AddBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_unique<VertexBuffer>( gfx,mesh.vertices ) );

	auto pvs = std::make_unique<VertexShader>( gfx,L"VertexShader.cso" );
	auto pvsb = pvs->GetBlob();
	AddBind( std::move( pvs ) );

	AddBind( std::make_unique<InputLayout>( gfx,pvsb,mesh.vertices.GetLayout().GetD3DLayout() ) );

	AddBind( std::make_unique<PixelShader>( gfx,L"PixelShader.cso" ) );

	AddBind( std::make_unique<PixelConstantBuffer<LightBuffer>>( gfx ) );

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

		if ( ImGui::Button( "Reset Position" ) )
		{
			ResetPosition();
		}

		if ( ImGui::SliderInt( "Mesh subdivisions",&nSubdivisions,0,4 ) )
		{
			UpdateMesh();
		}

		ImGui::End();
	}
}

void TestSphere::BindLight( Graphics& gfx ) noexcept
{
	auto lcbData = lightCBuf;
	DirectX::XMStoreFloat3(
		&lcbData.lightPos,
		DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3( &lightCBuf.lightPos ),
			gfx.GetCamera()
		)
	);
	auto pcb = QueryBindable<PixelConstantBuffer<LightBuffer>>();
	pcb->Update( gfx,lcbData );
}

void TestSphere::UpdateMesh() noexcept
{
	auto mesh = Sphere::MakeIcoSphere( nSubdivisions );

	auto pvb = QueryBindable<VertexBuffer>();
	pvb->UpdateBuffer( gfx,mesh.vertices );

	auto pib = QueryBindable<IndexBuffer>();
	pib->UpdateBuffer( gfx,mesh.indices );
}

void TestSphere::ResetPosition() noexcept
{
	pos = { 0.0f,0.0f,0.0f };
}
