#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>

#pragma comment( lib,"D3DCompiler.lib" )

VertexShader::VertexShader( Graphics& gfx,const std::wstring& filePath )
    :
    path( filePath )
{
    INFOMAN( gfx );

    GFX_THROW_INFO( D3DReadFileToBlob( filePath.c_str(),&pBlob ) );

    GFX_THROW_INFO( GetDevice( gfx )->CreateVertexShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pVertexShader ) );
}

void VertexShader::Bind( Graphics & gfx ) noexcept( !IS_DEBUG )
{
    INFOMAN_ONLY( gfx );

    GFX_THROW_INFO_ONLY( GetContext( gfx )->VSSetShader( pVertexShader.Get(),nullptr,0u ) );
}

ID3DBlob* VertexShader::GetBlob() noexcept
{
    return pBlob.Get();
}

std::string VertexShader::GenerateUID( const std::wstring& filePath ) noexcept
{
    using namespace std::string_literals;
    std::string path{ filePath.begin(),filePath.end() };
    return typeid( VertexShader ).name() + "#"s + path;
}

std::string VertexShader::GetUID() const noexcept
{
    return GenerateUID( path );
}
