#include "IndexedTriangleList.h"
#include <cassert>

IndexedTriangleList::IndexedTriangleList( VertexData verts,std::vector<unsigned short> inds )
	:
	vertices( std::move( verts ) ),
	indices( std::move( inds ) )
{
	assert( vertices.Size() > 2 );
	assert( inds.size() % 3 == 0 && "Number of indices should be divisible by 3" );
}

void IndexedTriangleList::Transform( DirectX::FXMMATRIX transformation ) noexcept
{
	namespace dx = DirectX;
	using ElType = VertexLayout::ElementType;
	for ( int i = 0; i < vertices.Size(); ++i )
	{
		auto& pos = vertices[i].Attr<ElType::Position3D>();
		dx::XMStoreFloat3(
			&pos,
			dx::XMVector3Transform(
				dx::XMLoadFloat3( &pos ),
				transformation
			)
		);
	}
}
