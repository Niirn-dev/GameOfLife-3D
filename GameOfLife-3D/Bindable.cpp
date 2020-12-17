#include "Bindable.h"
#include <stdexcept>

namespace wrl = Microsoft::WRL;

ID3D11Device* Bindable::GetDevice( Graphics& gfx ) noexcept
{
	return gfx.pDevice.Get();
}

ID3D11DeviceContext* Bindable::GetContext( Graphics& gfx ) noexcept
{
	return gfx.pContext.Get();
}

DxgiInfoManager& Bindable::GetInfoManager( Graphics& gfx )
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error( "You've done goofed. Attempting to get info manager in release configuration" );
#endif // !NDEBUG
}