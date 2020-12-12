#include "Window.h"
#include <cassert>
#include <sstream>
#include "WindowThrowMacros.h"

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
		throw WND_LAST_EXCEPT();
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
		throw WND_LAST_EXCEPT();
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
		throw WND_LAST_EXCEPT();
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

/*********** EXCEPTION DEFINITIONS ***********/
std::string Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
	char* pMsgBuf = nullptr;

	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,hr,MAKELANGID( LANG_NEUTRAL,SUBLANG_DEFAULT ),
		reinterpret_cast<LPSTR>( &pMsgBuf ),0,nullptr
	);

	if ( nMsgLen == 0 )
	{
		return "Unidentified error code";
	}

	std::string errorString = pMsgBuf;

	LocalFree( pMsgBuf );

	return errorString;
}

Window::HrException::HrException( int line,const char* file,HRESULT hr )
	:
	Exception( line,file ),
	hr( hr )
{}

const char* Window::HrException::what() const noexcept
{
	std::stringstream oss;
	oss << "[Type] " << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription()
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Niirn Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode( hr );
}
/********* EXCEPTION DEFINITIONS END *********/
