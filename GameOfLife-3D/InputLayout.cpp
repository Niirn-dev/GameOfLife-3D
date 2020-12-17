#include "InputLayout.h"
#include "GraphicsThrowMacros.h"

InputLayout::InputLayout( Graphics& gfx,ID3DBlob* pBlob,const std::vector<D3D11_INPUT_ELEMENT_DESC>& ieDesc )
{
	INFOMAN( gfx );

	GFX_THROW_INFO( GetDevice( gfx )->CreateInputLayout(
		ieDesc.data(),
		(UINT)ieDesc.size(),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	) );
}

void InputLayout::Bind( Graphics & gfx ) noexcept( !IS_DEBUG )
{
	INFOMAN( gfx );

	GFX_THROW_INFO_ONLY( GetContext( gfx )->IASetInputLayout( pInputLayout.Get() ) );
}
