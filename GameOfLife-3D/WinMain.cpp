#include "WinFlags.h"
#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE	_In_		,//hInstance,
	HINSTANCE	_In_opt_	,//hPrevInstance,
	LPSTR		_In_		,//lpCmdLine,
	int			_In_		//nCmdShow
)
{
	Window w{ 800,600,"Tasty tests" };

	while ( true )
	{
		if ( const auto ecode = w.ProcessMessages() )
		{
			return *ecode;
		}
	}

	return -1;
}