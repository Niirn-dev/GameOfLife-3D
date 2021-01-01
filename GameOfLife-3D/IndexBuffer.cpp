#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"

IndexBuffer::IndexBuffer( Graphics& gfx,const std::vector<unsigned short>& indices )
    :
    count( (UINT)indices.size() )
{
    CreateBuffer( gfx,indices );
}

void IndexBuffer::Bind( Graphics & gfx ) noexcept( !IS_DEBUG )
{
    INFOMAN_ONLY( gfx );

    GFX_THROW_INFO_ONLY( GetContext( gfx )->IASetIndexBuffer( 
        pIndexBuffer.Get(),
        DXGI_FORMAT_R16_UINT,
        0u
    ) );
}

void IndexBuffer::UpdateBuffer( Graphics& gfx,const std::vector<unsigned short>& indices )
{
    pIndexBuffer.Reset();
    count = (UINT)indices.size();

    CreateBuffer( gfx,indices );
}

UINT IndexBuffer::GetCount() const noexcept
{
    return count;
}

void IndexBuffer::CreateBuffer( Graphics& gfx,const std::vector<unsigned short>& indices )
{
    INFOMAN( gfx );

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = UINT( indices.size() * sizeof( unsigned short ) );
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0u;
    desc.MiscFlags = 0u;
    desc.StructureByteStride = sizeof( unsigned short );
    D3D11_SUBRESOURCE_DATA srd = {};
    srd.pSysMem = indices.data();

    GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &desc,&srd,&pIndexBuffer ) );
}
