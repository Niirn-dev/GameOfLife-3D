#include "TestSphere.h"
#include "BindableCommon.h"
#include "Sphere.h"

TestSphere::TestSphere( Graphics& gfx )
{
	const auto mesh = Sphere::MakeIcosahedral();

	AddBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_unique<VertexBuffer>( gfx,mesh.vertices ) );

	auto pvs = std::make_unique<VertexShader>( gfx,L"VertexShader.cso" );
	auto pvsb = pvs->GetBlob();
	AddBind( std::move( pvs ) );

	std::vector<D3D11_INPUT_ELEMENT_DESC> ieDesc = {
		{ "Position",0u,DXGI_FORMAT_R32G32B32_FLOAT,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0u }
	};
	AddBind( std::make_unique<InputLayout>( gfx,pvsb,ieDesc ) );

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
