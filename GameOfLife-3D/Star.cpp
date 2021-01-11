#include "Star.h"

Star::Star( Graphics& gfx,
			const DirectX::XMFLOAT3& modelColor,
			const DirectX::XMFLOAT3& diffuseColor,
			int nSubdivisions,float radius,
			unsigned short nPlanets )
	:
	pPointLight( std::make_unique<PointLight>( gfx,modelColor,diffuseColor,nSubdivisions,radius ) )
{
	for ( auto i = 0u; i < nPlanets; ++i )
	{
		AddPlanet( gfx );
	}
}

void Star::AddPlanet( Graphics & gfx ) noexcept( !IS_DEBUG )
{
	planetPtrs.push_back( std::make_unique<Planet>( gfx,0 ) );
}

void Star::Update( float dt ) noexcept
{
	for ( auto& pp : planetPtrs )
	{
		pp->Update( dt );
	}
}

void Star::BindLightAndDraw( Graphics & gfx ) noexcept( !IS_DEBUG )
{
	pPointLight->BindLightBuffer( gfx );
	pPointLight->Draw( gfx );

	for ( auto& pp : planetPtrs )
	{
		pp->Draw( gfx );
	}
}

void Star::SpawnLightControlWindow() noexcept
{
	pPointLight->SpawnControlWindow();
}
