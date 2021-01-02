#include "Topology.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

Topology::Topology( Graphics& gfx,D3D11_PRIMITIVE_TOPOLOGY type )
	:
	type( type )
{}

void Topology::Bind( Graphics & gfx ) noexcept( !IS_DEBUG )
{
	INFOMAN_ONLY( gfx );

	GFX_THROW_INFO_ONLY( GetContext( gfx )->IASetPrimitiveTopology( type ) );
}

std::shared_ptr<Bindable> Topology::Resolve( Graphics& gfx,D3D11_PRIMITIVE_TOPOLOGY type ) noexcept( !IS_DEBUG )
{
	return BindableCodex::Resolve<Topology>( gfx,type );
}

std::string Topology::GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type ) noexcept
{
	using namespace std::string_literals;
	return typeid( Topology ).name() + "#"s + std::to_string( type );
}

std::string Topology::GetUID() const noexcept
{
	return GenerateUID( type );
}
