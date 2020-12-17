#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader( Graphics& gfx,const std::wstring& filePath );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};