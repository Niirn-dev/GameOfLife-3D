#include "WinFlags.h"
#include "Window.h"
#include "NiiException.h"
#include <sstream>

int CALLBACK WinMain(
	HINSTANCE	_In_		hInstance,
	HINSTANCE	_In_opt_	hPrevInstance,
	LPSTR		_In_		lpCmdLine,
	int			_In_		nCmdShow
)
{
	try
	{
		Window w{ 800,600,"Game of Life" };
		std::string title = "";

		while ( true )
		{
			if ( const auto ecode = w.ProcessMessages() )
			{
				return *ecode;
			}

			while ( !w.kbd.IsKeyEmpty() )
			{
				const auto e = w.kbd.ReadKey();
				if ( e->IsPress() && e->GetCode() == VK_BACK )
				{
					if ( !title.empty() )
					{
						title.pop_back();
					}
				}
			}

			while ( !w.kbd.IsCharEmpty() )
			{
				if ( const auto c = *w.kbd.ReadChar(); c >= ' ' && c <= '}' )
				{
					title += c;
				}

				w.SetTitle( title );
			}
		}
	}
	catch ( const NiiException& e )
	{
		MessageBox( nullptr,e.what(),e.GetType(),MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( const std::exception& e )
	{
		MessageBox( nullptr,e.what(),"Standard exception",MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( ... )
	{
		MessageBox( nullptr,"No details availible","Unknown exception",MB_OK | MB_ICONERROR );
	}

	return -1;
}