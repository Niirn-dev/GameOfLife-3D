#include "InputLayout.h"
#include "GraphicsThrowMacros.h"

InputLayout::InputLayout( Graphics& gfx,const VertexLayout& layout,ID3DBlob* pBlob )
	:
	vxLayout( layout )
{
	INFOMAN( gfx );

	const auto ieDesc = layout.GetD3DLayout();

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
	INFOMAN_ONLY( gfx );

	GFX_THROW_INFO_ONLY( GetContext( gfx )->IASetInputLayout( pInputLayout.Get() ) );
}

std::string InputLayout::GetUID() const noexcept
{
	return GenerateUID( vxLayout );
}
