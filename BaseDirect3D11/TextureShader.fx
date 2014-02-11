cbuffer cbWorldMatrix : register(b0)
{
	matrix gWorldMatrix;
};

cbuffer cbViewMatrix : register(b1)
{
	matrix gViewMatrix;
};

cbuffer cbProjMatrix : register(b2)
{
	matrix gProjMatrix;
};

Texture2D gtxtDiffuse : register(t0);

SamplerState gSamplerState : register(s0);

struct VS_TEXTURE_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct PS_TEXTURE_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

PS_TEXTURE_INPUT VSTexture(VS_TEXTURE_INPUT input)
{
	PS_TEXTURE_INPUT output = (PS_TEXTURE_INPUT)0;

	output.position = float4(input.position.xyz, 1.0f);
	matrix mtxWorldViewProj = mul(gWorldMatrix, gViewMatrix);
	mtxWorldViewProj = mul(mtxWorldViewProj, gProjMatrix);
	output.position = mul(output.position, mtxWorldViewProj);
	output.color = input.color;
	output.texcoord = input.texcoord;
	
	return output;
}

float4 PSTexture(PS_TEXTURE_INPUT input) : SV_Target
{
	float4 diffuseColor = gtxtDiffuse.Sample(gSamplerState, input.texcoord);

	return diffuseColor;
}
