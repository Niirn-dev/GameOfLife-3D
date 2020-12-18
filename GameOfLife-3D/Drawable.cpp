#include "Drawable.h"
#include "IndexBuffer.h"
#include <cassert>

void Drawable::Draw( Graphics& gfx ) noexcept( !IS_DEBUG )
{
	assert( pIndexBuffer != nullptr && "Must have an index buffer pointer filled" );
	for ( auto& b : bindPtrs )
	{
		b->Bind( gfx );
	}

	gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind( std::unique_ptr<Bindable> bind ) noexcept
{
	if ( auto pi = dynamic_cast<IndexBuffer*>( bind.get() ) )
	{
		assert( pIndexBuffer == nullptr && "Attempting to bind second index buffer. Only one index buffer is allowed" );
		pIndexBuffer = pi;
	}

	bindPtrs.push_back( std::move( bind ) );
}
