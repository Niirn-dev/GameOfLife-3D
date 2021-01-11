cbuffer transforms
{
	matrix worldView;
	matrix worldViewProj;
};

float4 main( float3 pos : Position,float3 n : Normal ) : SV_Position
{
	return mul( float4( pos,1.0f ),worldViewProj );
}