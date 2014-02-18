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
	matrix gProjMatrix; //C0~C3
	matrix gInvProjMatrix; //C4~C7
	//float gFarPlane : packoffset(c8.x);
};

Texture2D gtxtDiffuse : register(t0);
Texture2D gtxtDepth : register(t1);

SamplerState gSamplerState : register(s0);

struct VS_SSDECAL_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct PS_SSDECAL_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 viewLay : POSITION;
	float4 clipPosition : POSITION1;
};

float3 GetCameraPos()
{
	return float3( -gViewMatrix[3][0], -gViewMatrix[3][1], -gViewMatrix[3][2] );
}

float4x4 GetInvViewMatrix()
{
	return float4x4(float4(gViewMatrix[0][0], gViewMatrix[0][1], gViewMatrix[0][2],0),
					float4(gViewMatrix[1][0], gViewMatrix[1][1], gViewMatrix[1][2],0),
					float4(gViewMatrix[2][0], gViewMatrix[2][1], gViewMatrix[2][2],0),
					float4(-gViewMatrix[0][3], -gViewMatrix[1][3], -gViewMatrix[2][3],1));
}


float4 LinearDepthToWorldPos(float depth, float3 viewLay)
{
	float fLookDistance;

	fLookDistance = dot(float3(0.0f, 0.0f ,1.0f), viewLay);

	float3 worldPos = depth * viewLay / fLookDistance;

	worldPos = mul(worldPos, (float3x3)GetInvViewMatrix());
	worldPos += GetCameraPos();

	return float4( worldPos, 1.0f );
}

//depth most be Proj.z / Proj.w
float ConvertToViewZ(float depth)
{
	float viewZ = gProjMatrix[3][2];
	viewZ /= depth - gProjMatrix[2][2];
	return viewZ;
}

float4x4 GetInvWorldMatrix()
{
	return float4x4(float4(gWorldMatrix[0][0], gWorldMatrix[1][0], gWorldMatrix[2][0], 0),
					float4(gWorldMatrix[0][1], gWorldMatrix[1][1], gWorldMatrix[2][1], 0),
					float4(gWorldMatrix[0][2], gWorldMatrix[1][2], gWorldMatrix[2][2], 0),
					float4(-gWorldMatrix[3][0], -gWorldMatrix[3][1], -gWorldMatrix[3][2], 1));
}

float2 ProjToScreen(float4 projPos)
{
	float2 screenPos = projPos.xy / projPos.w;
	screenPos = screenPos * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	return screenPos;
}

PS_SSDECAL_INPUT VSSSDecal(VS_SSDECAL_INPUT input)
{
	PS_SSDECAL_INPUT output = (PS_SSDECAL_INPUT)0;

	matrix mtxWorldView = mul(gWorldMatrix, gViewMatrix);
	float4 viewLay = mul(float4(input.position, 1.0f), mtxWorldView);
	output.position = mul(viewLay, gProjMatrix);

	output.viewLay = viewLay.xyz / viewLay.w;

	output.color = input.color;
	output.texcoord = input.texcoord;
	output.clipPosition = output.position;

	return output;
}

float4 PSSSDecal(PS_SSDECAL_INPUT input) : SV_Target
{
	float4 depth = gtxtDepth.Sample(gSamplerState, ProjToScreen(input.clipPosition));
	float4 pixelWorldPos = LinearDepthToWorldPos(ConvertToViewZ(depth.x), input.viewLay);
	pixelWorldPos = mul(pixelWorldPos, GetInvWorldMatrix());

	float3 ObjectAbs = abs(pixelWorldPos.xyz);
	clip( 0.5f - ObjectAbs );

	input.texcoord.x = pixelWorldPos.x + 0.5f;
	input.texcoord.y = -pixelWorldPos.y + 0.5f;

	float4 diffuseColor = gtxtDiffuse.Sample(gSamplerState, input.texcoord);

	return diffuseColor;
}