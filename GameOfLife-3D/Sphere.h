#pragma once

#include <DirectXMath.h>
#include <algorithm>
#include <iterator>
#include "IndexedTriangleList.h"
#include "NiiMath.h"

class Sphere
{
private:
	static constexpr float X = 0.525731112119133606f;
	static constexpr float Z = 0.850650808352039932f;
	static constexpr float N = 0.0f;

	using TriangleIndices = std::tuple<unsigned short,unsigned short,unsigned short>;
	using VertTriIdxPair = std::pair<std::vector<DirectX::XMFLOAT3>,std::vector<TriangleIndices>>;

	static VertTriIdxPair MakeIcosahedron() noexcept
	{
		std::vector<DirectX::XMFLOAT3> vertices = {
			{-X,N,Z},{X,N,Z},{-X,N,-Z},{X,N,-Z},
			{N,Z,X},{N,Z,-X},{N,-Z,X},{N,-Z,-X},
			{Z,X,N},{-Z,X,N},{Z,-X,N},{-Z,-X,N}
		};

		std::vector<TriangleIndices> triIndices = {
			{0,1,4},{0,4,9},{9,4,5},{4,8,5},{4,1,8},
			{8,1,10},{8,10,3},{5,8,3},{5,3,2},{2,3,7},
			{7,3,10},{7,10,6},{7,6,11},{11,6,0},{0,6,1},
			{6,10,1},{9,11,0},{9,2,11},{9,5,2},{7,11,2}
		};

		return { std::move( vertices ),std::move( triIndices ) };
	}
	static std::vector<TriangleIndices> SubdivideTriangles( std::vector<DirectX::XMFLOAT3>& vertices,const std::vector<TriangleIndices>& triIndices ) noexcept
	{
		std::vector<TriangleIndices> newTriangles;
		newTriangles.reserve( 4 * triIndices.size() );

		for ( const auto& ti : triIndices )
		{
			auto tris = Subdivide( vertices,ti );
			std::move( tris.begin(),tris.end(),std::back_inserter( newTriangles ) );
		}

		return std::move( newTriangles );
	}
	static std::vector<TriangleIndices> Subdivide( std::vector<DirectX::XMFLOAT3>& vertices,const TriangleIndices& triangle ) noexcept
	{
		namespace dx = DirectX;

		// indices of the original triangle
		const auto i0 = std::get<0>( triangle );
		const auto i1 = std::get<1>( triangle );
		const auto i2 = std::get<2>( triangle );

		// get handles for vertices of the triangle
		const auto& v0 = dx::XMLoadFloat3( &vertices[i0] );
		const auto& v1 = dx::XMLoadFloat3( &vertices[i1] );
		const auto& v2 = dx::XMLoadFloat3( &vertices[i2] );

		// get handles for edges
		const auto& edge0 = dx::XMVectorSubtract( v1,v0 );
		const auto& edge1 = dx::XMVectorSubtract( v2,v1 );
		const auto& edge2 = dx::XMVectorSubtract( v0,v2 );

		// get new vertices. normalize to keep vertices on the surface of the sphere
		const auto v3 = dx::XMVector3Normalize( dx::XMVectorAdd( v0,dx::XMVectorScale( edge0,0.5f ) ) );
		const auto v4 = dx::XMVector3Normalize( dx::XMVectorAdd( v1,dx::XMVectorScale( edge1,0.5f ) ) );
		const auto v5 = dx::XMVector3Normalize( dx::XMVectorAdd( v2,dx::XMVectorScale( edge2,0.5f ) ) );

		// add new vertices to the rest and store their indices
		const auto i3 = (unsigned short)vertices.size();
		vertices.push_back( {} );
		dx::XMStoreFloat3( &vertices.back(),v3 );

		const auto i4 = (unsigned short)vertices.size();
		vertices.push_back( {} );
		dx::XMStoreFloat3( &vertices.back(),v4 );

		const auto i5 = (unsigned short)vertices.size();
		vertices.push_back( {} );
		dx::XMStoreFloat3( &vertices.back(),v5 );

		// arrange indices of new triangles
		std::vector<TriangleIndices> newTriangleIndices;
		newTriangleIndices.reserve( 4 );

		newTriangleIndices.emplace_back( i0,i3,i5 );
		newTriangleIndices.emplace_back( i3,i1,i4 );
		newTriangleIndices.emplace_back( i3,i4,i5 );
		newTriangleIndices.emplace_back( i5,i4,i2 );

		return std::move( newTriangleIndices );
	}
public:
	static IndexedTriangleList MakeIcoSphere( int nSubdivisions = 0,float radius = 1.0f ) noexcept
	{
		namespace dx = DirectX;

		// get model of base icosahendron
		auto [vertices,triIndices] = MakeIcosahedron();

		// subdivide triangles of the icosahedron
		for ( int i = 0; i < nSubdivisions; ++i )
		{
			triIndices = std::move( SubdivideTriangles( vertices,triIndices ) );
		}

		// transform vector of vertices to dynamic vertex type
		VertexData vd{ VertexLayout{}
			.Append( VertexLayout::ElementType::Position3D )
			.Append( VertexLayout::ElementType::Normal )
		};
		for ( const auto& v : vertices )
		{
			// since generated mesh is that of unit sphere no need to normalize
			const auto vNorm = dx::XMLoadFloat3( &v );
			const auto vPos = dx::XMVector3Transform(
				dx::XMLoadFloat3( &v ),
				dx::XMMatrixScaling( radius,radius,radius )
			);
			vd.EmplaceBack(
				*reinterpret_cast<const dx::XMFLOAT3*>( &vPos ),
				*reinterpret_cast<const dx::XMFLOAT3*>( &vNorm )
			);
		}

		// transform triangle indices to regular index list
		std::vector<unsigned short> indices;
		indices.reserve( triIndices.size() * 3 );
		for ( const auto& ti : triIndices )
		{
			indices.push_back( std::get<0>( ti ) );
			indices.push_back( std::get<1>( ti ) );
			indices.push_back( std::get<2>( ti ) );
		}

		return { std::move( vd ),std::move( indices ) };
	}
};