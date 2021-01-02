#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader( Graphics& gfx,const std::wstring& filePath );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;

	static std::shared_ptr<Bindable> Resolve( Graphics& gfx,const std::wstring& filePath ) noexcept( !IS_DEBUG );
	static std::string GenerateUID( const std::wstring& filePath ) noexcept;
	std::string GetUID() const noexcept override;
private:
	std::wstring path;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};