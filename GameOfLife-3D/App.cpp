#include "App.h"

App::App()
	:
	wnd( 800,600,"Game of Life" ),
	sphere( wnd.Gfx() )
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

	sphere.Draw( wnd.Gfx() );
}
