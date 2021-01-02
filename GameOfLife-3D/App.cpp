#include "App.h"
#include "imgui/imgui.h"

App::App()
	:
	wnd( 1600,900,"Game of Life" ),
	sphere( wnd.Gfx() ),
	light( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,wnd.Gfx().GetAspectRatio(),0.5f,40.0f ) );
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
	sphere.SpawnControlWindow();
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

	sphere.Update( 1.0f / 60.0f );

	light.BindLightBuffer( wnd.Gfx() );
	sphere.Draw( wnd.Gfx() );
}
