#include "TransformCBuf.h"
#include "Drawable.h"
#include "GraphicsThrowMacros.h"

TransformCBuf::TransformCBuf( Graphics& gfx,const Drawable& parent )
	:
	parent( parent ),
	pTransformBuffer( std::make_unique<VertexConstantBuffer<Transforms>>( gfx ) )
{
}

void TransformCBuf::Bind( Graphics& gfx ) noexcept( !IS_DEBUG )
{
	UpdateBindImpl( gfx,GetTransforms( gfx ) );
}

TransformCBuf::Transforms TransformCBuf::GetTransforms( Graphics& gfx ) const noexcept
{
	auto worldView = parent.GetTransformXM() * gfx.GetCamera();
	return { 
		DirectX::XMMatrixTranspose( worldView ),
		DirectX::XMMatrixTranspose( 
			worldView * 
			gfx.GetProjection()
		)
	};
}

void TransformCBuf::UpdateBindImpl( Graphics& gfx,const Transforms& tfs ) noexcept( !IS_DEBUG )
{
	pTransformBuffer->Update( gfx,tfs );
	pTransformBuffer->Bind( gfx );
}
