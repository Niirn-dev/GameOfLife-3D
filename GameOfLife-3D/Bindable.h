#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual ~Bindable() = default;
	virtual void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) = 0;

protected:
	ID3D11Device* GetDevice( Graphics& gfx ) noexcept;
	ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept;
	DxgiInfoManager& GetInfoManager( Graphics& gfx );
};