#include "IndexedTriangleList.h"
#include <cassert>

IndexedTriangleList::IndexedTriangleList( std::vector<DirectX::XMFLOAT3> verts,std::vector<unsigned short> inds )
	:
	vertices( std::move( verts ) ),
	indices( std::move( inds ) )
{
	assert( inds.size() % 3 == 0 && "Number of indices should be divisible by 3" );
}

void IndexedTriangleList::Transform( DirectX::FXMMATRIX transformation ) noexcept
{
	namespace dx = DirectX;
	for ( auto& v : vertices )
	{
		dx::XMStoreFloat3(
			&v,
			dx::XMVector3Transform(
				dx::XMLoadFloat3( &v ),
				transformation
			)
		);
	}
}
