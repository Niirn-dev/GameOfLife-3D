#pragma once

#include "BaseSphereDrawable.h"

class PhongSphere : public BaseSphereDrawable
{
public:
	PhongSphere( Graphics& gfx,int nSubdivisions = 2,float radius = 1.0f );
};