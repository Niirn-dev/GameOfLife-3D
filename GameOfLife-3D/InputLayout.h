#pragma once

#include "Bindable.h"
#include "Vertex.h"

class InputLayout : public Bindable
{
public:
	InputLayout( Graphics& gfx,const VertexLayout& layout,ID3DBlob* pBlob );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;

	template<typename...Ignore>
	static std::string GenerateUID( const VertexLayout& layout,Ignore&&... ignore ) noexcept
	{
		using namespace std::string_literals;
		return typeid( InputLayout ).name() + "#"s + layout.GetCode();
	}
	std::string GetUID() const noexcept override;
private:
	const VertexLayout& vxLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};