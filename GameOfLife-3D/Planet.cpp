#include "Planet.h"
#include <algorithm>
#include <iterator>
#include <functional>

// init static instance of planet factory
Planet::Factory Planet::Factory::fctry{};

/*************** PLANET FACTORY ****************/
Planet::Factory::Factory() noexcept
	:
	rng( std::random_device{}() ),
	subdivDist( minSubdiv,maxSubdiv ),
	rDist( meanRadius,sigmaRadius ),
	nOrbitDist( minOrbit,maxOrbit ),
	angleDist( minAngle,maxAngle ),
	angleSpeedDist( minAngleSpeed,maxAngleSpeed ),
	nMoonsDist( minMoons,maxMoons )
{}

Planet::Factory& Planet::Factory::Get() noexcept
{
	return fctry;
}

std::unique_ptr<Planet> Planet::Factory::PlanetPtr( Graphics& gfx,int level ) const noexcept
{
	return std::make_unique<Planet>( gfx,level );
}

int Planet::Factory::Subdivision( int level ) noexcept
{
	return subdivDist( rng );
}

float Planet::Factory::Radius( int level ) noexcept
{
	return rDist( rng ) - (float)level;
}

float Planet::Factory::Orbit( int level ) noexcept
{
	return (float)nOrbitDist( rng ) * ( 10.0f / (float)( level + 1 ) );
}

float Planet::Factory::Angle( int level ) noexcept
{
	return angleDist( rng );
}

float Planet::Factory::AngleSpeed( int level ) noexcept
{
	return angleSpeedDist( rng );
}

int Planet::Factory::Moons( int level ) noexcept
{
	return std::max( 0,nMoonsDist( rng ) - 2 * level );
}
/************* PLANET FACTORY END **************/

Planet::Planet( Graphics& gfx,
				int level )
	:
	radius( std::max( minRadius,Factory::Get().Radius( level ) ) ),
	startPos( 0.0f,0.0f,Factory::Get().Orbit( level ) ),
	dPlanetPitch( Factory::Get().AngleSpeed( level ) ),
	dPlanetYaw( Factory::Get().AngleSpeed( level ) ),
	dPlanetRoll( Factory::Get().AngleSpeed( level ) ),
	dPitch( Factory::Get().AngleSpeed( level ) ),
	dYaw( Factory::Get().AngleSpeed( level ) ),
	dRoll( Factory::Get().AngleSpeed( level ) ),
	pitch( Factory::Get().Angle( level ) ),
	yaw( Factory::Get().Angle( level ) ),
	roll( Factory::Get().Angle( level ) ),
	mesh( gfx,Factory::Get().Subdivision( level ),radius )
{
	std::generate_n( std::back_inserter( moonPtrs ),
					 Factory::Get().Moons( level ),
					 [&]() 
					 {
						 return Factory::Get().PlanetPtr( gfx,level + 1 );
					 } );
}

void Planet::Update( float dt ) noexcept
{
	UpdatePosition( dt );
	mesh.IncOrientation( dPlanetPitch,dPlanetYaw,dPlanetRoll );
}

void Planet::Draw( Graphics& gfx ) noexcept( !IS_DEBUG )
{
	mesh.Draw( gfx );
	for ( auto& mp : moonPtrs )
	{
		mp->Draw( gfx );
	}
}

void Planet::UpdatePosition( float dt ) noexcept
{
	namespace dx = DirectX;

	pitch += dPitch * dt;
	yaw += dYaw * dt;
	roll += dRoll * dt;

	WrapAngles();

	const auto pos = dx::XMVector3Transform(
		dx::XMLoadFloat3( &startPos ),
		dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll )
	);
	mesh.SetPosition( pos );
}

void Planet::WrapAngles() noexcept
{
	wrap_angle( pitch );
	wrap_angle( yaw );
	wrap_angle( roll );
}
