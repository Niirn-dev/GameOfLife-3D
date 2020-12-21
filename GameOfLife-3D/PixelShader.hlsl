cbuffer cbuf
{
	float4 c[6];
};

float4 main( uint id : SV_PrimitiveID ) : SV_TARGET
{
	return c[id % 5];
}