#pragma once

#include <DirectXMath.h>
#include "IndexedTriangleList.h"
#include "NiiMath.h"

class Sphere
{
private:
	static constexpr float X = 0.525731112119133606f;
	static constexpr float Z = 0.850650808352039932f;
	static constexpr float N = 0.0f;

	using TriangleIndices = std::tuple<unsigned short,unsigned short,unsigned short>;

	static std::pair<std::vector<DirectX::XMFLOAT3>,std::vector<TriangleIndices>> MakeIcosahedron() noexcept
	{
		std::vector<DirectX::XMFLOAT3> vertices = {
			{-X,N,Z},{X,N,Z},{-X,N,-Z},{X,N,-Z},
			{N,Z,X},{N,Z,-X},{N,-Z,X},{N,-Z,-X},
			{Z,X,N},{-Z,X,N},{Z,-X,N},{-Z,-X,N}
		};

		std::vector<TriangleIndices> triIndices = {
			{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
			{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
			{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
			{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
		};

		return { std::move( vertices ),std::move( triIndices ) };
	}
public:
	static IndexedTriangleList MakeIcoSphere() noexcept
	{
		auto [vertices,triIndices] = MakeIcosahedron();

		std::vector<unsigned short> indices;
		indices.reserve( triIndices.size() * 3 );
		for ( const auto& ti : triIndices )
		{
			indices.push_back( std::get<0>( ti ) );
			indices.push_back( std::get<1>( ti ) );
			indices.push_back( std::get<2>( ti ) );
		}

		return { std::move( vertices ),std::move( indices ) };
	}
};