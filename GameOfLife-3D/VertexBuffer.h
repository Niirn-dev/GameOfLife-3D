#pragma once

#include "Bindable.h"
#include "Vertex.h"
#include <DirectXMath.h>

class VertexBuffer : public Bindable
{
public:
	VertexBuffer( Graphics& gfx,const VertexData& vertices );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT stride;
	UINT offset;
};