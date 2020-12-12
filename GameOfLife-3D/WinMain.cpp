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

		while ( true )
		{
			if ( const auto ecode = w.ProcessMessages() )
			{
				return *ecode;
			}

			while ( !w.mouse.IsEmpty() )
			{
				static int wheelClickCount = 0;
				const auto e = w.mouse.Read();
				if ( e->GetType() == Mouse::Event::Type::WheelUp )
				{
					const auto [mx,my] = e->GetPos();
					std::stringstream oss;
					oss << "Wheel up: " << ++wheelClickCount
						<< " | Cursor position: (" << mx << "," << my << ")";
					w.SetTitle( oss.str() );
				}
				if ( e->GetType() == Mouse::Event::Type::WheelDown )
				{
					const auto [mx,my] = e->GetPos();
					std::stringstream oss;
					oss << "Wheel down: " << --wheelClickCount
						<< " | Cursor position: (" << mx << "," << my << ")";
					w.SetTitle( oss.str() );
				}
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