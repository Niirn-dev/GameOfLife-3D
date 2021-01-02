#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>

#pragma comment( lib,"D3DCompiler.lib" )

PixelShader::PixelShader( Graphics& gfx,const std::wstring& filePath )
	:
	path( filePath )
{
	INFOMAN( gfx );

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO( D3DReadFileToBlob( filePath.c_str(),&pBlob ) );

	GFX_THROW_INFO( GetDevice( gfx )->CreatePixelShader( 
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pPixelShader
	) );
}

void PixelShader::Bind( Graphics& gfx ) noexcept( !IS_DEBUG )
{
	INFOMAN_ONLY( gfx );

	GFX_THROW_INFO_ONLY( GetContext( gfx )->PSSetShader( pPixelShader.Get(),nullptr,0u ) );
}

std::string PixelShader::GenerateUID( const std::wstring& filePath ) noexcept
{
	using namespace std::string_literals;
	std::string path{ filePath.begin(),filePath.end() };
	return typeid( PixelShader ).name() + "#"s + path;
}

std::string PixelShader::GetUID() const noexcept
{
	return GenerateUID( path );
}
