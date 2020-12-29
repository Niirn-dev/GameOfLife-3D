#include "TestSphere.h"
#include "BindableCommon.h"
#include "Sphere.h"
#include "imgui/imgui.h"

TestSphere::TestSphere( Graphics& gfx,int nSubdivisions,float radius )
	:
	gfx( gfx ),
	nSubdiv( nSubdivisions ),
	r( radius ),
	mesh( gfx,nSubdivisions,radius )
{}

void TestSphere::Update( float dt ) noexcept
{
	mesh.IncOrientation( dAngle * dt,dAngle * dt,dAngle * dt );
}

void TestSphere::Draw( Graphics& gfx ) noexcept( !IS_DEBUG )
{
	mesh.Draw( gfx );
}

void TestSphere::SpawnControlWindow() noexcept
{
	if ( ImGui::Begin( "Sphere" ) )
	{
		{
			ImGui::Text( "Position" );
			auto pos = mesh.GetPosition();
			bool posChanged = false;
			posChanged |= ImGui::SliderFloat( "X",&pos.x,-10.0f,10.0f,"%.1f" );
			posChanged |= ImGui::SliderFloat( "Y",&pos.y,-10.0f,10.0f,"%.1f" );
			posChanged |= ImGui::SliderFloat( "Z",&pos.z,-10.0f,10.0f,"%.1f" );
			if ( posChanged )
			{
				mesh.SetPosition( pos );
			}
		}
		
		if ( ImGui::Button( "Reset Position" ) )
		{
			ResetPosition();
		}

		if ( ImGui::SliderFloat( "Radius",&r,0.1f,3.0f,"%.1f" ) )
		{
			mesh.SetRadius( r );
		}

		if ( ImGui::SliderInt( "Mesh subdivisions",&nSubdiv,0,4 ) )
		{
			mesh.SetMeshSubdivisions( gfx,nSubdiv );
		}

		ImGui::End();
	}
}

void TestSphere::ResetPosition() noexcept
{
	mesh.SetPosition( { 0.0f,0.0f,0.0f } );
}
