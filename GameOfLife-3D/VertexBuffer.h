#pragma once

#include "Bindable.h"
#include <DirectXMath.h>

class VertexBuffer : public Bindable
{
public:
	VertexBuffer( Graphics& gfx,const std::vector<DirectX::XMFLOAT3>& vertices );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT stride;
	UINT offset;
};