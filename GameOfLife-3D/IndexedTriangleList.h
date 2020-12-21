#pragma once

#include <DirectXMath.h>
#include <vector>

class IndexedTriangleList
{
public:
	IndexedTriangleList( std::vector<DirectX::XMFLOAT3> verts,std::vector<unsigned short> inds );
	void Transform( DirectX::FXMMATRIX transformation ) noexcept;

public:
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<unsigned short> indices;
};