#include "Topology.h"
#include "GraphicsThrowMacros.h"

Topology::Topology( Graphics& gfx,D3D11_PRIMITIVE_TOPOLOGY type )
	:
	type( type )
{}

void Topology::Bind( Graphics & gfx ) noexcept( !IS_DEBUG )
{
	INFOMAN_ONLY( gfx );

	GFX_THROW_INFO_ONLY( GetContext( gfx )->IASetPrimitiveTopology( type ) );
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
