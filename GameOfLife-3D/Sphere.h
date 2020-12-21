#pragma once

#include <DirectXMath.h>
#include "IndexedTriangleList.h"
#include "NiiMath.h"

class Sphere
{
public:
	static IndexedTriangleList MakeIcosahedral()
	{
		namespace dx = DirectX;
		const auto base = dx::XMVectorSet( 1.0f,0.0f,0.0f,0.0f );

		std::vector<dx::XMFLOAT3> vertices;
		vertices.reserve( 14 );

		for ( unsigned short iLat = 0; iLat < 2; ++iLat )
		{
			const auto latBase = dx::XMVector3Transform(
				base,
				dx::XMMatrixRotationZ( ( PI / 3.0f ) * ( iLat + 1 ) ) *
				dx::XMMatrixRotationX( ( PI / 6.0f ) * iLat )
			);
			for ( unsigned short iLong = 0; iLong < 6; ++iLong )
			{
				vertices.push_back( {} );
				dx::XMStoreFloat3(
					&vertices.back(),
					dx::XMVector3Transform(
						latBase,
						dx::XMMatrixRotationX( ( PI / 3.0f ) * iLong )
					)
				);
			}
		}

		const auto iNorthPole = vertices.size();
		vertices.push_back( {} );
		dx::XMStoreFloat3(
			&vertices.back(),
			base
		);
		const auto iSouthPole = vertices.size();
		vertices.push_back( {} );
		dx::XMStoreFloat3(
			&vertices.back(),
			dx::XMVectorNegate( base )
		);

		std::vector<unsigned short> indices;
		const auto calcIdx = []( unsigned short iLat,unsigned short iLong )
		{
			return iLat * (unsigned short)6 + iLong;
		};
		for ( unsigned short iLat = 0; iLat < 1; ++iLat )
		{
			for ( unsigned short iLong = 0; iLong < 5; ++iLong )
			{
				indices.push_back( calcIdx( iLat,iLong ) );
				indices.push_back( calcIdx( iLat,iLong + 1 ) );
				indices.push_back( calcIdx( iLat + 1,iLong ) );

				indices.push_back( calcIdx( iLat,iLong + 1 ) );
				indices.push_back( calcIdx( iLat + 1,iLong + 1 ) );
				indices.push_back( calcIdx( iLat + 1,iLong ) );
			}
			// wrap band
			indices.push_back( calcIdx( iLat,5 ) );
			indices.push_back( calcIdx( iLat,0 ) );
			indices.push_back( calcIdx( iLat + 1,5 ) );

			indices.push_back( calcIdx( iLat,0 ) );
			indices.push_back( calcIdx( iLat + 1,0 ) );
			indices.push_back( calcIdx( iLat + 1,5 ) );
		}

		// cap fans
		for ( unsigned short iLong = 0; iLong < 5; ++iLong )
		{
			// north pole
			indices.push_back( iNorthPole );
			indices.push_back( calcIdx( 0,iLong + 1 ) );
			indices.push_back( calcIdx( 0,iLong ) );
			// south pole
			indices.push_back( iSouthPole );
			indices.push_back( calcIdx( 1,iLong ) );
			indices.push_back( calcIdx( 1,iLong + 1 ) );
		}
		// wrap cap fans
		// north pole
		indices.push_back( iNorthPole );
		indices.push_back( calcIdx( 0,0 ) );
		indices.push_back( calcIdx( 0,5 ) );
		// south pole
		indices.push_back( iSouthPole );
		indices.push_back( calcIdx( 1,5 ) );
		indices.push_back( calcIdx( 1,0 ) );

		return { std::move( vertices ),std::move( indices ) };
	}
};