#pragma once
#include "WinFlags.h"
#include <optional>
#include <string>
#include "NiiException.h"
#include "Mouse.h"

class Window
{
private:
	class Exception : public NiiException
	{
		using NiiException::NiiException;
	public:
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr );
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const char* wndClassName = "BasicWindow";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window( int width,int height,const char* name );
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;

	void SetTitle( const std::string& title );
	std::optional<int> ProcessMessages() noexcept;
private:
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;

public:
	Mouse mouse;
private:
	int width;
	int height;
	HWND hWnd;
};