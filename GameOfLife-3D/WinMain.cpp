#include "WinFlags.h"
#include "App.h"
#include "NiiException.h"

int CALLBACK WinMain(
	HINSTANCE	_In_		hInstance,
	HINSTANCE	_In_opt_	hPrevInstance,
	LPSTR		_In_		lpCmdLine,
	int			_In_		nCmdShow
)
{
	try
	{
		return App{}.Go();
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