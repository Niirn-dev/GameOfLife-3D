#include "App.h"

App::App()
	:
	wnd( 800,600,"Game of Life" )
{}

int App::Go()
{
	while ( true )
	{
		if ( const auto ecode = wnd.ProcessMessages() )
		{
			return *ecode;
		}

		DoFrame();
	}
}

void App::DoFrame()
{
	while ( !wnd.kbd.IsKeyEmpty() )
	{
		const auto e = wnd.kbd.ReadKey();
		if ( e->IsPress() && e->GetCode() == VK_BACK )
		{
			if ( !title.empty() )
			{
				title.pop_back();
			}
		}
	}

	while ( !wnd.kbd.IsCharEmpty() )
	{
		if ( const auto c = *wnd.kbd.ReadChar(); c >= ' ' && c <= '}' )
		{
			title += c;
		}

		wnd.SetTitle( title );
	}
}
