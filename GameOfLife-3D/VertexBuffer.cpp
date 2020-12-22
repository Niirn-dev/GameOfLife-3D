#include "VertexBuffer.h"
#include "GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

VertexBuffer::VertexBuffer( Graphics& gfx,const VertexData& vertices )
	:
	stride( (UINT)vertices.GetLayout().SizeBytes() ),
	offset( 0u )
{
	INFOMAN( gfx );

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = UINT( vertices.SizeBytes() );
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0u;
	desc.MiscFlags = 0u;
	desc.StructureByteStride = stride;
	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = vertices.GetData();

	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &desc,&srd,&pVertexBuffer ) );
}

void VertexBuffer::Bind( Graphics& gfx ) noexcept( !IS_DEBUG )
{
	INFOMAN( gfx );

	GFX_THROW_INFO_ONLY( GetContext( gfx )->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset ) );
}
