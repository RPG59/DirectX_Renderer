struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix mWorldViewProj;
	float4  vSomeVectorThatMayBeNeededByASpecificShader;
	float fSomeFloatThatMayBeNeededByASpecificShader;
	float fTime;
	float fSomeFloatThatMayBeNeededByASpecificShader2;
	float fSomeFloatThatMayBeNeededByASpecificShader3;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = position;
	output.color = color * fTime;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}