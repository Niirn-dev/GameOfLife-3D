#include "TestTriangle.h"
#include "BindableCommon.h"

TestTriangle::TestTriangle( Graphics& gfx )
{
	std::vector<DirectX::XMFLOAT3> vertices;
	vertices.reserve( 3 );
	vertices.push_back( { 0.0f,0.5f,2.5f } );
	vertices.push_back( { 0.5f,-0.5f,2.5f } );
	vertices.push_back( { -0.5f,-0.5f,2.5f } );

	AddBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_unique<VertexBuffer>( gfx,vertices ) );

	auto pvs = std::make_unique<VertexShader>( gfx,L"VertexShader.cso" );
	auto pvsb = pvs->GetBlob();
	AddBind( std::move( pvs ) );

	std::vector<D3D11_INPUT_ELEMENT_DESC> ieDesc = {
		{ "Position",0u,DXGI_FORMAT_R32G32B32_FLOAT,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0u }
	};
	AddBind( std::make_unique<InputLayout>( gfx,pvsb,ieDesc ) );

	AddBind( std::make_unique<PixelShader>( gfx,L"PixelShader.cso" ) );

	std::vector<unsigned short> indices;
	indices.reserve( 3 );
	indices.emplace_back( 0 );
	indices.emplace_back( 1 );
	indices.emplace_back( 2 );

	AddBind( std::make_unique<IndexBuffer>( gfx,indices ) );

	struct PSConstBuffer
	{
		DirectX::XMFLOAT4 c = { 0.6f,0.6f,0.9f,1.0f };
	} psCBuf;
	AddBind( std::make_unique<PixelConstantBuffer<PSConstBuffer>>( gfx,psCBuf ) );

	AddBind( std::make_unique<TransformCBuf>( gfx,*this ) );
}

void TestTriangle::Update( DirectX::XMFLOAT3 dPos ) noexcept
{
	namespace dx = DirectX;
	dx::XMStoreFloat3(
		&pos,
		dx::XMVectorAdd(
			dx::XMLoadFloat3( &pos ),
			dx::XMLoadFloat3( &dPos )
		)
	);
}

DirectX::XMMATRIX TestTriangle::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation( pos.x,pos.y,pos.z );
}
