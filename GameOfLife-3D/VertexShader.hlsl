cbuffer transforms
{
	matrix worldView;
	matrix worldViewProj;
};

struct VSOut
{
	float3 viewPos : Position;
	float3 viewNorm : Normal;
	float4 pos : SV_POSITION;
};

VSOut main( float3 pos : Position,float3 norm : Normal )
{
	VSOut vso;
	vso.viewPos = (float3)mul( float4( pos,1.0f ),worldView );
	vso.viewNorm = mul( norm,(float3x3)worldView );
	vso.pos = mul( float4( pos,1.0f ),worldViewProj );
	return vso;
}