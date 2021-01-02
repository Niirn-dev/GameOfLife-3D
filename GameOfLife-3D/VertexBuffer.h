#pragma once

#include "Bindable.h"
#include "Vertex.h"
#include <DirectXMath.h>

class VertexBuffer : public Bindable
{
public:
	VertexBuffer( Graphics& gfx,const std::string& tag,const VertexData& vertices );
	VertexBuffer( Graphics& gfx,const VertexData& vertices );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;

	template<typename...Ignore>
	static std::string GenerateUID( const std::string& tag,Ignore&&... ignore ) noexcept
	{
		using namespace std::string_literals;
		return typeid( VertexBuffer ).name() + "#"s + tag;
	}
	std::string GetUID() const noexcept override;

	void UpdateBuffer( Graphics& gfx,const VertexData& vertices );
private:
	void CreateBuffer( Graphics& gfx,const VertexData& vertices );

private:
	std::string tag;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT stride;
	UINT offset;
};