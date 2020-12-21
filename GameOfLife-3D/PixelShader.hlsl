float4 main( uint id : SV_PrimitiveID ) : SV_TARGET
{
	float shade = ( id % 10 ) * 0.1f;
	return float4( shade,shade,shade,1.0f );
}