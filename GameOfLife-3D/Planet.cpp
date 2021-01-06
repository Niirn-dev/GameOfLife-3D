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

Planet::OrbitAttributes Planet::Factory::OrbitAttr( int level ) noexcept
{
	OrbitAttributes oa = {};

	oa.index = OrbitIndex( level );
	oa.attr.radius = OrbitRadius( level,oa.index );

	oa.attr.angle = GenerateAngles();
	oa.attr.angleSpeed = GenerateAngleSpeeds();

	return std::move( oa );
}

Planet::Attributes Planet::Factory::PlanetAttr( int level,int orbitIndex ) noexcept
{
	Attributes a = {};

	a.radius = PlanetRadius( level,orbitIndex );

	a.angle = GenerateAngles();
	a.angleSpeed = GenerateAngleSpeeds();

	return std::move( a );
}

int Planet::Factory::MoonCount( int level,int orbit ) noexcept
{
	return std::max( 0,nMoonsDist( rng ) + orbit - 2 * level );
}

int Planet::Factory::Subdivision( int level ) noexcept
{
	return subdivDist( rng );
}

/************* PRIVATE PORTION **************/

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

Planet::Angles Planet::Factory::GenerateAngles() noexcept
{
	Angles angles = {};

	angles.pitch = angleDist( rng );
	angles.yaw = angleDist( rng );
	angles.roll = angleDist( rng );

	return std::move( angles );
}

Planet::Angles Planet::Factory::GenerateAngleSpeeds() noexcept
{
	Angles aSpeed = {};

	aSpeed.pitch = angleSpeedDist( rng );
	aSpeed.yaw = angleSpeedDist( rng );
	aSpeed.roll = angleSpeedDist( rng );

	return std::move( aSpeed );
}
/************* PLANET FACTORY END **************/

Planet::Planet( Graphics& gfx,
				int level )
{
	auto& f = Factory::Get();

	orbitAttrs = f.OrbitAttr( level );
	planetAttrs = f.PlanetAttr( level,orbitAttrs.index );

	pMesh = std::make_unique<PhongSphere>( gfx,f.Subdivision( level ),planetAttrs.radius );

	std::generate_n( std::back_inserter( moonPtrs ),
					 f.MoonCount( level,orbitAttrs.index ),
					 [&]() 
					 {
						 return f.PlanetPtr( gfx,level + 1 );
					 } );
}

void Planet::Update( float dt ) noexcept
{
	UpdatePosition( dt );
	pMesh->IncOrientation( 
		planetAttrs.angleSpeed.pitch,
		planetAttrs.angleSpeed.yaw,
		planetAttrs.angleSpeed.roll
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

	orbitAttrs.attr.angle.pitch += orbitAttrs.attr.angleSpeed.pitch * dt;
	orbitAttrs.attr.angle.yaw += orbitAttrs.attr.angleSpeed.yaw * dt;
	orbitAttrs.attr.angle.roll += orbitAttrs.attr.angleSpeed.roll * dt;

	WrapAngles();

	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet( 0.0f,0.0f,orbitAttrs.attr.radius,0.0f ),
		dx::XMMatrixRotationRollPitchYaw( 
			orbitAttrs.attr.angle.pitch,
			orbitAttrs.attr.angle.yaw,
			orbitAttrs.attr.angle.roll
		)
	);
	pMesh->SetPosition( pos );
}

void Planet::WrapAngles() noexcept
{
	wrap_angle( orbitAttrs.attr.angle.pitch );
	wrap_angle( orbitAttrs.attr.angle.yaw );
	wrap_angle( orbitAttrs.attr.angle.roll );
}
