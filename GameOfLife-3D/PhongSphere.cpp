#include "PhongSphere.h"

namespace dx = DirectX;

PhongSphere::PhongSphere( Graphics& gfx,int nSubdivisions,float radius )
	:
	BaseSphereDrawable{ gfx,L"PhongVS.cso",L"PhongPS.cso",nSubdivisions,radius }
{}
