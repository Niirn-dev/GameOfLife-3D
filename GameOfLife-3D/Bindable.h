#pragma once
#include "Graphics.h"
#include <string>
#include <typeinfo>
#include <memory>

class Bindable
{
public:
	virtual ~Bindable() = default;
	virtual void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) = 0;
	virtual std::string GetUID() const noexcept
	{
		assert( false && "GetUID not defined" );
		using namespace std::string_literals;
		return "UNDEFINED"s;
	};

protected:
	ID3D11Device* GetDevice( Graphics& gfx ) noexcept;
	ID3D11DeviceContext* GetContext( Graphics& gfx ) noexcept;
	DxgiInfoManager& GetInfoManager( Graphics& gfx );
};