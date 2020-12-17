#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout( Graphics& gfx,ID3DBlob* pBlob,const std::vector<D3D11_INPUT_ELEMENT_DESC>& ieDesc );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};