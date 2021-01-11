#include "BaseSphereDrawable.h"
#include "BindableCommon.h"
#include "Sphere.h"
#include "NiiMath.h"

namespace dx = DirectX;

BaseSphereDrawable::BaseSphereDrawable( Graphics& gfx,const std::wstring& vertexShader,const std::wstring& pixelShader,int nSubdivisions,float radius )
	:
	r( radius )
{
	using namespace std::string_literals;

	auto mesh = Sphere::MakeIcoSphere( nSubdivisions );
	const auto meshTag = typeid( Sphere ).name() + "#"s + std::to_string( nSubdivisions );

	AddBind( Topology::Resolve( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( VertexBuffer::Resolve( gfx,meshTag,mesh.vertices ) );

	auto pvs = VertexShader::Resolve( gfx,vertexShader );
	auto pvsb = std::static_pointer_cast<VertexShader>( pvs )->GetBlob();
	AddBind( std::move( pvs ) );

	AddBind( InputLayout::Resolve( gfx,mesh.vertices.GetLayout(),pvsb ) );

	AddBind( PixelShader::Resolve( gfx,pixelShader ) );

	AddBind( IndexBuffer::Resolve( gfx,meshTag,mesh.indices ) );

	AddBind( std::make_shared<TransformCBuf>( gfx,*this ) );
}

void BaseSphereDrawable::SetParentTransformation( DirectX::FXMMATRIX transform ) noexcept
{
	parentTransform = transform;
}

DirectX::XMMATRIX BaseSphereDrawable::GetTransformXM() const noexcept
{
	return dx::XMMatrixScaling( r,r,r ) *
		dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		parentTransform *
		dx::XMMatrixTranslation( pos.x,pos.y,pos.z );
}

const DirectX::XMFLOAT3& BaseSphereDrawable::GetPosition() const noexcept
{
	return pos;
}

void BaseSphereDrawable::SetPosition( const DirectX::XMFLOAT3& pos_in ) noexcept
{
	pos = pos_in;
}

void BaseSphereDrawable::SetPosition( DirectX::FXMVECTOR pos_in ) noexcept
{
	dx::XMStoreFloat3( &pos,pos_in );
}

void BaseSphereDrawable::IncPosition( const DirectX::XMFLOAT3& dPos ) noexcept
{
	dx::XMStoreFloat3(
		&pos,
		dx::XMVectorAdd(
			dx::XMLoadFloat3( &pos ),
			dx::XMLoadFloat3( &dPos )
		)
	);
}

void BaseSphereDrawable::SetOrientation( float pitch_in,float yaw_in,float roll_in ) noexcept
{
	pitch = pitch_in;
	yaw = yaw_in;
	roll = roll_in;

	WrapAngles();
}

void BaseSphereDrawable::IncOrientation( float dPitch,float dYaw,float dRoll ) noexcept
{
	pitch += dPitch;
	yaw += dYaw;
	roll += dRoll;

	WrapAngles();
}

void BaseSphereDrawable::SetRadius( float radius ) noexcept
{
	assert( radius > 0 && "Radius value should be a positive value" );
	r = radius;
}

void BaseSphereDrawable::SetMeshSubdivisions( Graphics& gfx,int nSubdivisions ) noexcept
{
	auto mesh = Sphere::MakeIcoSphere( nSubdivisions );

	auto pvb = QueryBindable<VertexBuffer>();
	pvb->UpdateBuffer( gfx,mesh.vertices );

	auto pib = QueryBindable<IndexBuffer>();
	pib->UpdateBuffer( gfx,mesh.indices );
}

void BaseSphereDrawable::WrapAngles() noexcept
{
	wrap_angle( pitch );
	wrap_angle( yaw );
	wrap_angle( roll );
}
