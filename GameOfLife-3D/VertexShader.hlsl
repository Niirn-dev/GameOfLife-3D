cbuffer transforms
{
	matrix worldView;
	matrix worldViewProj;
};

float4 main( float3 pos : Position ) : SV_POSITION
{
	return mul( float4( pos, 1.0f ),worldViewProj );
}