#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader( Graphics& gfx,const std::wstring& filePath );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;
	ID3DBlob* GetBlob() noexcept;

	static std::string GenerateUID( const std::wstring& filePath ) noexcept;
	std::string GetUID() const noexcept override;
private:
	std::wstring path;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};