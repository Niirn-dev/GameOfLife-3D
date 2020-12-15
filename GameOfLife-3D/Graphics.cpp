#include "Graphics.h"
#include "dxerr.h"
#include <ios>
#include <sstream>
#include <assert.h>
#include "GraphicsThrowMacros.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment( lib,"d3d11.lib" )
#pragma comment( lib,"D3DCompiler.lib" )

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

	UINT createDeviceFlags = 0u;
#ifndef NDEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	// for checking results of d3d functions
	HRESULT hr;

	GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		nullptr,
		0u,
		D3D11_SDK_VERSION,
		&scDesc,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	) );

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO( pSwap->GetBuffer( 0,__uuidof( ID3D11Resource ),&pBackBuffer ) );
	GFX_THROW_INFO( pDevice->CreateRenderTargetView( pBackBuffer.Get(),nullptr,&pTarget ) );

	D3D11_VIEWPORT vp = {};
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports( 1u,&vp );

	GFX_THROW_INFO_ONLY( pContext->OMSetRenderTargets( 1u,pTarget.GetAddressOf(),nullptr ) );
}

void Graphics::BeginFrame( float r,float g,float b ) noexcept
{
	const float color[] = { r,g,b,1.0f };
	pContext->ClearRenderTargetView( pTarget.Get(),color );
}

void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif

	if ( FAILED( hr = pSwap->Present( 1u,0u ) ) )
	{
		if ( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
		}
		else
		{
			throw GFX_EXCEPT( hr );
		}
	}
}

void Graphics::DrawTriangle()
{
	namespace dx = DirectX;
	namespace wrl = Microsoft::WRL;
	HRESULT hr;

	std::vector<dx::XMFLOAT3> vertices;
	vertices.reserve( 3 );
	vertices.push_back( { 0.0f,0.5f,0.5f } );
	vertices.push_back( { 0.5f,-0.5f,0.5f } );
	vertices.push_back( { -0.5f,-0.5f,0.5f } );

	GFX_THROW_INFO_ONLY( pContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = UINT( vertices.size() * sizeof( dx::XMFLOAT3 ) );
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0u;
	bufDesc.MiscFlags = 0u;
	bufDesc.StructureByteStride = sizeof( dx::XMFLOAT3 );
	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = vertices.data();

	GFX_THROW_INFO( pDevice->CreateBuffer( &bufDesc,&srd,&pVertexBuffer ) );
	UINT stride = sizeof( dx::XMFLOAT3 );
	UINT offset = 0u;
	GFX_THROW_INFO_ONLY( pContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset ) );

	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO( D3DReadFileToBlob( L"VertexShader.cso",&pBlob ) );

	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO( pDevice->CreateVertexShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pVertexShader ) );
	GFX_THROW_INFO_ONLY( pContext->VSSetShader( pVertexShader.Get(),nullptr,0u ) );

	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	D3D11_INPUT_ELEMENT_DESC ieDesc[] = {
		{ "Position",0u,DXGI_FORMAT_R32G32B32_FLOAT,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0u }
	};
	GFX_THROW_INFO( pDevice->CreateInputLayout(
		ieDesc,
		1u,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	) );
	GFX_THROW_INFO_ONLY( pContext->IASetInputLayout( pInputLayout.Get() ) );

	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO( D3DReadFileToBlob( L"PixelShader.cso",&pBlob ) );
	GFX_THROW_INFO( pDevice->CreatePixelShader( pBlob->GetBufferPointer(),pBlob->GetBufferSize(),nullptr,&pPixelShader ) );
	GFX_THROW_INFO_ONLY( pContext->PSSetShader( pPixelShader.Get(),nullptr,0u ) );


	std::vector<unsigned short> indices;
	indices.reserve( 3 );
	indices.emplace_back( 0 );
	indices.emplace_back( 1 );
	indices.emplace_back( 2 );

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	bufDesc.ByteWidth = UINT( indices.size() * sizeof( unsigned short ) );
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.CPUAccessFlags = 0u;
	bufDesc.MiscFlags = 0u;
	bufDesc.StructureByteStride = sizeof( unsigned short );

	srd.pSysMem = indices.data();

	GFX_THROW_INFO( pDevice->CreateBuffer( &bufDesc,&srd,&pIndexBuffer ) );
	GFX_THROW_INFO_ONLY( pContext->IASetIndexBuffer( pIndexBuffer.Get(),DXGI_FORMAT_R16_UINT,0u ) );

	GFX_THROW_INFO_ONLY( pContext->DrawIndexed( (UINT)indices.size(),0u,0u ) );
}

/*********** EXCEPTION DEFINITIONS ***********/
Graphics::HrException::HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsgs ) noexcept
	:
	Exception( line,file ),
	hr( hr )
{
	for ( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if ( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::stringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription();
	if ( !info.empty() )
	{
		oss << "\n\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Niirn Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString( hr );
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription( hr,buf,sizeof( buf ) );
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}


Graphics::InfoException::InfoException( int line,const char* file,std::vector<std::string> infoMsgs ) noexcept
	:
	Exception( line,file )
{
	for ( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if ( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::stringstream oss;
	oss << GetType() << std::endl
		<< "\n\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Niirn Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Niirn Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
/********* EXCEPTION DEFINITIONS END *********/