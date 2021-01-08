#include "Planet.h"
#include <algorithm>
#include <iterator>
#include <cmath>

// init static instance of planet factory
Planet::Factory Planet::Factory::fctry{};

/*************** PLANET FACTORY ****************/
Planet::Factory::Factory() noexcept
	:
	rng( std::random_device{}() ),
	subdivDist( minSubdiv,maxSubdiv ),
	oIdxDist( minOrbit,maxOrbit ),
	raDist( meanRadiusAdj,sigmaRadiusAdj ),
	angleDist( minAngle,maxAngle ),
	angleSpeedDist( minAngleSpeed,maxAngleSpeed ),
	nMoonsDist( minMoons,maxMoons )
{}

Planet::Factory& Planet::Factory::Get() noexcept
{
	return fctry;
}

std::unique_ptr<Planet> Planet::Factory::PlanetPtr( Graphics& gfx,unsigned short level ) noexcept
{
	return std::make_unique<Planet>( gfx,level );
}

Planet::OrbitAttributes Planet::Factory::OrbitAttr( unsigned short level ) noexcept
{
	OrbitAttributes oa = {};
	auto& fGet = Factory::Get();

	oa.index = fGet.OrbitIndex( level );
	oa.attr.radius = fGet.OrbitRadius( level,oa.index );

	oa.attr.angle = fGet.GenerateAngles();
	oa.attr.angleSpeed = fGet.GenerateAngleSpeeds();

	return std::move( oa );
}

Planet::Attributes Planet::Factory::PlanetAttr( unsigned short level,unsigned short orbIdx ) noexcept
{
	Attributes a = {};
	auto& fGet = Factory::Get();

	a.radius = fGet.PlanetRadius( level,orbIdx );

	a.angle = fGet.GenerateAngles();
	a.angleSpeed = fGet.GenerateAngleSpeeds();

	return std::move( a );
}

unsigned short Planet::Factory::MoonCount( unsigned short level,unsigned short /*orbIdx*/ ) noexcept
{
	return std::max( 
		0u,
		Factory::Get().nMoonsDist( Factory::Get().rng ) / ( level + 1u )
	);
}

int Planet::Factory::Subdivision( unsigned short level ) noexcept
{
	return Factory::Get().subdivDist( Factory::Get().rng );
}

/************* PRIVATE PORTION **************/

unsigned short Planet::Factory::OrbitIndex( unsigned short level ) noexcept
{
	return std::max( 
		0u,
		oIdxDist( rng ) / ( level + 1u )
	);
}

float Planet::Factory::OrbitRadius( unsigned short level,unsigned short orbIdx ) noexcept
{
	return raDist( rng ) * std::pow( 2.0f,(float)orbIdx - 2.0f ) * std::pow( 10.0f,2.0f - (float)level );
}

float Planet::Factory::PlanetRadius( unsigned short level,unsigned short orbIdx ) noexcept
{
	const auto meanR = 1.0f / (float)( std::abs( avgOrbit - orbIdx ) + 1 );
	return meanR * raDist( rng );
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
				unsigned short level )
	:
	orbitAttrs( Factory::OrbitAttr( level ) ),
	planetAttrs( Factory::PlanetAttr( level,orbitAttrs.index ) ),
	pMesh( std::make_unique<PhongSphere>( gfx,Factory::Subdivision( level ),planetAttrs.radius ) )
{
	std::generate_n( std::back_inserter( moonPtrs ),
					 Factory::MoonCount( level,orbitAttrs.index ),
					 [&]() 
					 {
						 return Factory::PlanetPtr( gfx,level + 1 );
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
