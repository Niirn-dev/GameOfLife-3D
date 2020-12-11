#include "Window.h"
#include <cassert>

// WindowClass stuff
Window::WindowClass Window::WindowClass::wndClass;

const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

Window::WindowClass::WindowClass() noexcept
    :
    hInst( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx( &wc );
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( GetName(),GetInstance() );
}

// Window stuff
Window::Window( int width,int height,const char* name )
	:
	width( width ),
	height( height )
{
	// calculate desired client region rectangle
	RECT rect;
	rect.left = 100;
	rect.right = rect.left + width;
	rect.top = 100;
	rect.bottom = rect.top + height;
	if ( AdjustWindowRect( &rect,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE ) == 0 )
	{
		assert( "Couldn't adjust window rect" && false );
	}

	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,nullptr,WindowClass::GetInstance(),this
	);
	// check for error
	if ( hWnd == nullptr )
	{
		assert( "Couldn't create window" && false );
	}
	// newly created windows start off minimized
	ShowWindow( hWnd,SW_SHOWDEFAULT );
}

Window::~Window()
{
	DestroyWindow( hWnd );
}

void Window::SetTitle( const std::string& title )
{
	if ( SetWindowText( hWnd,title.c_str() ) )
	{
		assert( "Couldn't set window title" && false );
	}
}

std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	while ( PeekMessage( &msg,nullptr,0,0,PM_REMOVE ) )
	{
		if ( msg.message == WM_QUIT )
		{
			return (int)msg.wParam;
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return {};
}

LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	if ( msg == WM_NCCREATE )
	{
		// get pointer to create struct
		const auto createStruct = reinterpret_cast<CREATESTRUCT*>( lParam );
		// extract pointer to Window class instance from create params
		auto* const pWnd = static_cast<Window*>( createStruct->lpCreateParams );
		// store pointer to pointer to instance in user data
		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>( pWnd ) );
		// change msg process function to HandleMsgThunk
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk ) );
		// call the member message handling routine
		return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
	}

	return DefWindowProc( hWnd,msg,wParam,lParam );
}

LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	// retrieve pointer to Window instance
	auto* const pWnd = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd,GWLP_USERDATA ) );
	// call the member message handling routine
	return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
}

LRESULT Window::HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	switch ( msg )
	{
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	}

	return DefWindowProc( hWnd,msg,wParam,lParam );
}
