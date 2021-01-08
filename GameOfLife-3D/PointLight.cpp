#include "PointLight.h"
#include "BindableCommon.h"
#include "imgui/imgui.h"

PointLight::PointLight( Graphics& gfx )
	:
	mesh( gfx,4,2.5f ),
	pCBuff( std::make_unique<PixelConstantBuffer<LightBuffer>>( gfx ) )
{
	Reset();
}

void PointLight::Draw( Graphics& gfx ) const noexcept( !IS_DEBUG )
{
	mesh.SetPosition( lightCBuf.pos );
	mesh.Draw( gfx );
}

void PointLight::BindLightBuffer( Graphics& gfx ) const noexcept
{
	auto lcbData = lightCBuf;
	DirectX::XMStoreFloat3(
		&lcbData.pos,
		DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3( &lightCBuf.pos ),
			gfx.GetCamera()
		)
	);
	pCBuff->Update( gfx,lcbData );
	pCBuff->Bind( gfx );
}

void PointLight::SpawnControlWindow() noexcept
{
	if ( ImGui::Begin( "Point Light" ) )
	{
		ImGui::Text( "Position" );
		ImGui::SliderFloat( "X",&lightCBuf.pos.x,-30.0f,30.0f,"%.1f" );
		ImGui::SliderFloat( "Y",&lightCBuf.pos.y,30.0f,80.0f,"%.1f" );
		ImGui::SliderFloat( "Z",&lightCBuf.pos.z,-30.0f,30.0f,"%.1f" );

		ImGui::ColorEdit3( "Ambient",&lightCBuf.ambient.x );
		ImGui::ColorEdit3( "Diffuse",&lightCBuf.diffuse.x );
		ImGui::SliderFloat( "Diff. intensity",&lightCBuf.diffuseIntensity,0.00f,50.00f,"%.1f" );

		ImGui::Text( "Attenuation" );
		ImGui::SliderFloat( "Constant",&lightCBuf.attConst,0.01f,5.0f,"%.2f" );
		ImGui::SliderFloat( "Linear",&lightCBuf.attLin,0.001f,1.0f,"%.3f" );
		ImGui::SliderFloat( "Quadratic",&lightCBuf.attQuad,0.0001f,1.0f,"%.4f" );

		if ( ImGui::Button( "Reset" ) )
		{
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	lightCBuf = 
	{
		{ 0.0f,0.0f,0.0f },
		{ 0.012f,0.0f,0.008f },
		{ 1.0f,0.0f,0.0f },
		22.0f,
		1.0f,
		0.07f,
		0.017f
	};
}
