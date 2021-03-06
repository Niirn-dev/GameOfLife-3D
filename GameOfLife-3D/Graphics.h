#pragma once
#include "WinFlags.h"
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "NiiException.h"
#include "DxgiInfoManager.h"

class Graphics
{
	friend class Bindable;
public:
	class Exception : public NiiException
	{
		using NiiException::NiiException;
	};
	class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsgs = {} ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException( int line,const char* file,std::vector<std::string> infoMsgs ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics( HWND hWnd,int width,int height );
	~Graphics();
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;

	void BeginFrame( float r,float g,float b ) noexcept;
	void EndFrame();

	void DrawIndexed( UINT count ) noexcept( !IS_DEBUG );

	void SetProjection( DirectX::FXMMATRIX proj ) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	void SetCamera( DirectX::FXMMATRIX camTransform ) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;
private:
	DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX camera = DirectX::XMMatrixIdentity();

public:
	float GetAspectRatio() const noexcept;
private:
	int width;
	int height;

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
};
