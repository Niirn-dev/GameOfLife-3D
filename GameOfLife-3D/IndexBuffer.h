#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer( Graphics& gfx,const std::string& tag,const std::vector<unsigned short>& indices );
	IndexBuffer( Graphics& gfx,const std::vector<unsigned short>& indices );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;

	template<typename...Ignore>
	static std::string GenerateUID( const std::string& tag,Ignore&&... ignore ) noexcept
	{
		using namespace std::string_literals;
		return typeid( IndexBuffer ).name() + "#"s + tag;
	}
	std::string GetUID() const noexcept override;

	void UpdateBuffer( Graphics& gfx,const std::vector<unsigned short>& indices );
	UINT GetCount() const noexcept;
private:
	void CreateBuffer( Graphics& gfx,const std::vector<unsigned short>& indices );

private:
	std::string tag;
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};