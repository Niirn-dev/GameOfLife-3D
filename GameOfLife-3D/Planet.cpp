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

int Planet::Factory::OrbitIndex( int level ) noexcept
{
	return nOrbitDist( rng ) / (float)( level + 1 );
}

float Planet::Factory::PlanetRadius( int level,int orbit ) noexcept
{
	return rDist( rng ) - (float)level;
}

float Planet::Factory::OrbitRadius( int level,int orbit ) noexcept
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

int Planet::Factory::Moons( int level,int orbit ) noexcept
{
	return std::max( 0,nMoonsDist( rng ) + orbit - 2 * level );
}
/************* PLANET FACTORY END **************/

Planet::Planet( Graphics& gfx,
				int level )
{
	auto& f = Factory::Get();

	orbitParams.index = f.OrbitIndex( level );
	orbitParams.param.radius = f.OrbitRadius( level,orbitParams.index );
	orbitParams.param.angle.pitch = f.Angle( level );
	orbitParams.param.angle.yaw = f.Angle( level );
	orbitParams.param.angle.roll = f.Angle( level );
	orbitParams.param.angleSpeed.pitch = f.AngleSpeed( level );
	orbitParams.param.angleSpeed.yaw = f.AngleSpeed( level );
	orbitParams.param.angleSpeed.roll = f.AngleSpeed( level );

	planetParams.radius = f.PlanetRadius( level,orbitParams.index );
	planetParams.angle.pitch = f.Angle( level );
	planetParams.angle.yaw = f.Angle( level );
	planetParams.angle.roll = f.Angle( level );
	planetParams.angleSpeed.pitch = f.AngleSpeed( level );
	planetParams.angleSpeed.yaw = f.AngleSpeed( level );
	planetParams.angleSpeed.roll = f.AngleSpeed( level );

	pMesh = std::make_unique<PhongSphere>( gfx,f.Subdivision( level ),f.PlanetRadius( level,orbitParams.index ) );

	std::generate_n( std::back_inserter( moonPtrs ),
					 f.Moons( level,orbitParams.index ),
					 [&]() 
					 {
						 return f.PlanetPtr( gfx,level + 1 );
					 } );
}

void Planet::Update( float dt ) noexcept
{
	UpdatePosition( dt );
	pMesh->IncOrientation( 
		planetParams.angleSpeed.pitch,
		planetParams.angleSpeed.yaw,
		planetParams.angleSpeed.roll
	);

	// update moons
	for ( auto& mp : moonPtrs )
	{
		mp->Update( dt );
	}
}

void Planet::Draw( Graphics& gfx,DirectX::FXMMATRIX parentTransform ) noexcept( !IS_DEBUG )
{
	pMesh->SetParentTransformation( parentTransform );
	pMesh->Draw( gfx );

	const auto currentPos = pMesh->GetPosition();
	const auto currentTransform = parentTransform *
		DirectX::XMMatrixTranslation( currentPos.x,currentPos.y,currentPos.z );
	for ( auto& mp : moonPtrs )
	{
		mp->Draw( gfx,currentTransform );
	}
}

void Planet::UpdatePosition( float dt ) noexcept
{
	namespace dx = DirectX;

	orbitParams.param.angle.pitch += orbitParams.param.angleSpeed.pitch * dt;
	orbitParams.param.angle.yaw += orbitParams.param.angleSpeed.yaw * dt;
	orbitParams.param.angle.roll += orbitParams.param.angleSpeed.roll * dt;

	WrapAngles();

	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet( 0.0f,0.0f,orbitParams.param.radius,0.0f ),
		dx::XMMatrixRotationRollPitchYaw( 
			orbitParams.param.angle.pitch,
			orbitParams.param.angle.yaw,
			orbitParams.param.angle.roll
		)
	);
	pMesh->SetPosition( pos );
}

void Planet::WrapAngles() noexcept
{
	wrap_angle( orbitParams.param.angle.pitch );
	wrap_angle( orbitParams.param.angle.yaw );
	wrap_angle( orbitParams.param.angle.roll );
}
