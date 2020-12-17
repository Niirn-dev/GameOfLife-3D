#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>

#pragma comment( lib,"D3DCompiler.lib" )

VertexShader::VertexShader( Graphics& gfx,const std::wstring& filePath )
{
    INFOMAN( gfx );

    GFX_THROW_INFO( D3DReadFileToBlob( filePath.c_str(),&pBlob ) );

    GFX_THROW_INFO( GetDevice( gfx )->CreateVertexShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pVertexShader ) );
}

void VertexShader::Bind( Graphics & gfx ) noexcept( !IS_DEBUG )
{
    INFOMAN( gfx );

    GFX_THROW_INFO_ONLY( GetContext( gfx )->VSSetShader( pVertexShader.Get(),nullptr,0u ) );
}

ID3DBlob* VertexShader::GetBlob() noexcept
{
    return pBlob.Get();
}
