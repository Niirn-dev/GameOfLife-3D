#include "Graphics.h"
#include <assert.h>

#pragma comment( lib,"d3d11.lib" )

namespace wrl = Microsoft::WRL;

Graphics::Graphics( HWND hWnd,int width,int height )
	:
	width( width ),
	height( height )
{
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferDesc.Width = width;
	scDesc.BufferDesc.Height = height;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 0;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 1;
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = TRUE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = 0;

	const auto hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0u,
		D3D11_SDK_VERSION,
		&scDesc,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	if ( FAILED( hr ) )
	{
		assert( "Failed to creare device and swap chain" && false );
	}

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer( 0,__uuidof( ID3D11Resource ),&pBackBuffer );
	pDevice->CreateRenderTargetView( pBackBuffer.Get(),nullptr,&pTarget );

	D3D11_VIEWPORT vp = {};
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports( 1u,&vp );
}

void Graphics::BeginFrame( float r,float g,float b ) noexcept
{
	const float color[] = { r,g,b,1.0f };
	pContext->ClearRenderTargetView( pTarget.Get(),color );
}

void Graphics::EndFrame()
{
	pSwap->Present( 1u,0u );
}
