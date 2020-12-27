#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer( Graphics& gfx,const std::vector<unsigned short>& indices );
	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override;

	void UpdateBuffer( Graphics& gfx,const std::vector<unsigned short>& indices );
	UINT GetCount() const noexcept;
private:
	void CreateBuffer( Graphics& gfx,const std::vector<unsigned short>& indices );

private:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};