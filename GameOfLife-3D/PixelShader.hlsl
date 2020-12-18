cbuffer cbuf
{
	float4 c;
};

float4 main() : SV_TARGET
{
	return c;
}