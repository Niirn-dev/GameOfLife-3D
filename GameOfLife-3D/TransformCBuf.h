#pragma once

#include "Bindable.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>
#include <memory>

class TransformCBuf : public Bindable
{
	struct Transforms
	{
		DirectX::XMMATRIX worldProj = DirectX::XMMatrixIdentity();
	};
public:
	TransformCBuf( Graphics& gfx,const class Drawable& parent  );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;
private:
	Transforms GetTransforms( Graphics& gfx ) const noexcept;
	void UpdateBindImpl( Graphics& gfx,const Transforms& tfs ) noexcept( !IS_DEBUG );
private:
	const Drawable& parent;
	std::unique_ptr<VertexConstantBuffer<Transforms>> pTransformBuffer;
};