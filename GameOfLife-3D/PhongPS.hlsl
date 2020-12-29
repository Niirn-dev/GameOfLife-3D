cbuffer LightCBuf
{
	float3 lightViewPos;
	float3 ambient;
	float3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

float4 main( float3 viewPos : Position,float3 viewNorm : Normal,uint id : SV_PrimitiveID ) : SV_Target
{
	float shade = ( id % 10 ) * 0.1f;
	float3 matColor = float3( shade,shade,shade );

	float3 vToL = lightViewPos - viewPos;
	float dist = length( vToL );
	float3 dir = vToL / dist;

	float att = 1.0f / ( attQuad * dist * dist + attLin * dist + attConst );
	float3 diffuse = diffuseColor * att * diffuseIntensity * max( 0.0f,dot( dir,normalize( viewNorm ) ) );

	float3 finalColor = saturate( ( diffuse + ambient ) * matColor );

	return float4( finalColor,1.0f );
}