#include "SolidSphere.h"
#include "ConstantBuffers.h"

SolidSphere::SolidSphere( Graphics& gfx,const DirectX::XMFLOAT3& color,int nSubdivisions,float radius )
	:
	BaseSphereDrawable{ gfx,L"SolidVS.cso",L"SolidPS.cso",nSubdivisions,radius }
{
	pcbuff.matColor = color;
	AddBind( PixelConstantBuffer<MaterialCBuff>::Resolve( gfx,pcbuff,1u ) );
}

void SolidSphere::SetMaterialColor( Graphics& gfx,DirectX::XMFLOAT3 color ) noexcept
{
	pcbuff.matColor = color;

	auto cb = QueryBindable<PixelConstantBuffer<MaterialCBuff>>();
	cb->Update( gfx,pcbuff );
}
