cbuffer matCBuff : register( b1 )
{
	float3 matColor;
};

float4 main( uint id : SV_PrimitiveID ) : SV_Target
{
	float shade = ( id % 10 ) * 0.1f;
	return float4( matColor * shade,1.0f );
}