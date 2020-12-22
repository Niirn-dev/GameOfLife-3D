#pragma once

#include <DirectXMath.h>
#include <vector>
#include "Vertex.h"

class IndexedTriangleList
{
public:
	IndexedTriangleList( VertexData verts,std::vector<unsigned short> inds );
	void Transform( DirectX::FXMMATRIX transformation ) noexcept;

public:
	VertexData vertices;
	std::vector<unsigned short> indices;
};