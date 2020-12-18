#pragma once

#include "Bindable.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>
#include <memory>

class TransformCBuf : public Bindable
{
public:
	TransformCBuf( Graphics& gfx,const class Drawable& parent  );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;
private:
	const Drawable& parent;
	struct Transforms
	{
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	} tf;
	std::unique_ptr<VertexConstantBuffer<Transforms>> pTransformBuffer;
};