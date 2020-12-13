#pragma once
#include <wrl.h>
#include <d3d11.h>

class Graphics
{
public:
	Graphics( HWND hWnd,int width,int height );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;

	void BeginFrame( float r,float g,float b ) noexcept;
	void EndFrame();

private:
	int width;
	int height;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};
