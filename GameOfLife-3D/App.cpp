#include "App.h"
#include "imgui/imgui.h"

#include <random>

App::App()
	:
	wnd( 1600,900,"Game of Life" ),
	p( wnd.Gfx(),0 ),
	light( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,wnd.Gfx().GetAspectRatio(),0.5f,140.0f ) );
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
		DoFrame( timer.Mark() );
		wnd.Gfx().EndFrame();
	}
}

void App::DoFrame( float dt )
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
	p.Update( dt );

	light.BindLightBuffer( wnd.Gfx() );
	// light.Draw( wnd.Gfx() );
	p.Draw( wnd.Gfx() );
}
