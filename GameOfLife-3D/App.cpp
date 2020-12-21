#include "App.h"

App::App()
	:
	wnd( 800,600,"Game of Life" ),
	sphere( wnd.Gfx() )
{
	wnd.Gfx().SetProjection( DirectX::XMMatrixPerspectiveLH( 1.0f,3.0f / 4.0f,0.5f,40.0f ) );
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

	DirectX::XMFLOAT3 dPos = {};
	if ( wnd.kbd.IsKeyPressed( 'W' ) )
	{
		dPos.z += 0.5f / 60.0f;
	}
	if ( wnd.kbd.IsKeyPressed( 'S' ) )
	{
		dPos.z -= 0.5f / 60.0f;
	}
	if ( wnd.kbd.IsKeyPressed( 'A' ) )
	{
		dPos.x -= 0.5f / 60.0f;
	}
	if ( wnd.kbd.IsKeyPressed( 'D' ) )
	{
		dPos.x += 0.5f / 60.0f;
	}
	sphere.Update( dPos );

	sphere.Draw( wnd.Gfx() );
}
