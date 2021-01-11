#pragma once

#include "BaseSphereDrawable.h"

class SolidSphere : public BaseSphereDrawable
{
public:
	SolidSphere( Graphics& gfx,const DirectX::XMFLOAT3& color,int nSubdivisions,float radius );

	void SetMaterialColor( Graphics& gfx,DirectX::XMFLOAT3 color ) noexcept;
private:
	struct MaterialCBuff
	{
		DirectX::XMFLOAT3 matColor = { 1.0f,1.0f,1.0f };
		float padding = {};
	} pcbuff;
};