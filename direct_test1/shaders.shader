struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex: TEXCOORD0;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix mWorldViewProj;
	float fSomeFloatThatMayBeNeededByASpecificShader;
	float fTime;
	int curspX;
	int curspY;
};

cbuffer PS_CONSTANT_BUFFER : register(b1)
{
	int posx;
	int posy;
	int foo1;
	int foo2;
}
///////////////
Texture2D shaderTexture;
SamplerState SamplerType : register(S0);
///////////////////
VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 tex : TEXCOORD0)
{
	matrix Identity =
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	VOut output;
	output.position = mul(position, mWorldViewProj);
	output.color = color * fTime;
	output.tex = tex;
	//output.color.x = curspX / 1000.0;
	//output.color.y = curspY / 1000.0;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 tex : TEXCOORD0) : SV_TARGET
{
	float4 textureColor;
	textureColor = shaderTexture.Sample(SamplerType, tex);

	float intensity = 100;
	float foo = intensity / length(float2(position.x - posx, position.y - posy));
	//return float4(tex.r, color.gba) * foo;
	return textureColor * foo;
}
