#include "PhongSphere.h"
#include "BindableCommon.h"
#include "Sphere.h"
#include "NiiMath.h"

namespace dx = DirectX;

PhongSphere::PhongSphere( Graphics& gfx,int nSubdivisions,float radius )
	:
	r( radius )
{
	using namespace std::string_literals;

	auto mesh = Sphere::MakeIcoSphere( nSubdivisions );
	const auto meshTag = typeid( PhongSphere ).name() + "#"s + std::to_string( nSubdivisions );

	AddBind( Topology::Resolve( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( VertexBuffer::Resolve( gfx,meshTag,mesh.vertices ) );

	auto pvs = VertexShader::Resolve( gfx,L"PhongVS.cso" );
	auto pvsb = std::static_pointer_cast<VertexShader>( pvs )->GetBlob();
	AddBind( std::move( pvs ) );

	AddBind( InputLayout::Resolve( gfx,mesh.vertices.GetLayout(),pvsb ) );

	AddBind( PixelShader::Resolve( gfx,L"PhongPS.cso" ) );

	AddBind( IndexBuffer::Resolve( gfx,meshTag,mesh.indices ) );

	AddBind( std::make_shared<TransformCBuf>( gfx,*this ) );
}

void PhongSphere::SetParentTransformation( DirectX::FXMMATRIX transform ) noexcept
{
	parentTransform = transform;
}

DirectX::XMMATRIX PhongSphere::GetTransformXM() const noexcept
{
	return dx::XMMatrixScaling( r,r,r ) *
		dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		parentTransform *
		dx::XMMatrixTranslation( pos.x,pos.y,pos.z );
}

const DirectX::XMFLOAT3& PhongSphere::GetPosition() const noexcept
{
	return pos;
}

void PhongSphere::SetPosition( const DirectX::XMFLOAT3& pos_in ) noexcept
{
	pos = pos_in;
}

void PhongSphere::SetPosition( DirectX::FXMVECTOR pos_in ) noexcept
{
	dx::XMStoreFloat3( &pos,pos_in );
}

void PhongSphere::IncPosition( const DirectX::XMFLOAT3& dPos ) noexcept
{
	dx::XMStoreFloat3(
		&pos,
		dx::XMVectorAdd(
			dx::XMLoadFloat3( &pos ),
			dx::XMLoadFloat3( &dPos )
		)
	);
}

void PhongSphere::SetOrientation( float pitch_in,float yaw_in,float roll_in ) noexcept
{
	pitch = pitch_in;
	yaw = yaw_in;
	roll = roll_in;

	WrapAngles();
}

void PhongSphere::IncOrientation( float dPitch,float dYaw,float dRoll ) noexcept
{
	pitch += dPitch;
	yaw += dYaw;
	roll += dRoll;

	WrapAngles();
}

void PhongSphere::SetRadius( float radius ) noexcept
{
	assert( radius > 0 && "Radius value should be a positive value" );
	r = radius;
}

void PhongSphere::SetMeshSubdivisions( Graphics& gfx,int nSubdivisions ) noexcept
{
	auto mesh = Sphere::MakeIcoSphere( nSubdivisions );

	auto pvb = QueryBindable<VertexBuffer>();
	pvb->UpdateBuffer( gfx,mesh.vertices );

	auto pib = QueryBindable<IndexBuffer>();
	pib->UpdateBuffer( gfx,mesh.indices );
}

void PhongSphere::WrapAngles() noexcept
{
	wrap_angle( pitch );
	wrap_angle( yaw );
	wrap_angle( roll );
}
