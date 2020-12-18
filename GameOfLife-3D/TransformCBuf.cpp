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
	tf.world = parent.GetTransformXM();
	pTransformBuffer->Update( gfx,tf );
	pTransformBuffer->Bind( gfx );
}
