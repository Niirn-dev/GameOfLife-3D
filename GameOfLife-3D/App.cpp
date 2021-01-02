#include "App.h"
#include "imgui/imgui.h"

#include <random>
#include <algorithm>
#include <iterator>
#include "NiiMath.h"

#include "BindableCommon.h"
#include "Sphere.h"

App::App()
	:
	wnd( 1600,900,"Game of Life" ),
	light( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,wnd.Gfx().GetAspectRatio(),0.5f,40.0f ) );

	auto rng = std::mt19937( std::random_device{}() );
	auto sdDist = std::uniform_int_distribution<int>{ 0,4 };
	auto rDist = std::uniform_real_distribution<float>{ 0.2f,1.2f };
	auto orbitDist = std::normal_distribution<float>{ 7.0f,4.0f };
	auto aDist = std::uniform_real_distribution<float>{ 0.0f,2.0f * PI };
	auto asDist = std::uniform_real_distribution<float>{ -PI,PI };

	std::generate_n( std::back_inserter( spherePtrs ),nSpheres,
					 [&]() 
					 {
						 return std::make_unique<TestSphere>( wnd.Gfx(),rng,sdDist,rDist,orbitDist,aDist,asDist );
					 } );
}

int App::Go()
{
	while ( true )
	{
		if ( const auto ecode = wnd.ProcessMessages() )
		{
			return *ecode;
		}

		wnd.Gfx().BeginFrame( 0.12f,0.0f,0.08f );
		DoFrame();
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame()
{
	light.SpawnControlWindow();
	cam.SpawnControlWindow();

	while ( !wnd.kbd.IsKeyEmpty() )
	{
		const auto e = wnd.kbd.ReadKey();
		if ( e->IsPress() && e->GetCode() == 'I' )
		{
			wnd.ToggleCursorCapture();
		}
	}

	wnd.Gfx().SetCamera( cam.GetMatrix() );

	light.BindLightBuffer( wnd.Gfx() );
	for ( auto& ps : spherePtrs )
	{
		ps->Update( 1.0f / 60.0f );
		ps->Draw( wnd.Gfx() );
	}
}
