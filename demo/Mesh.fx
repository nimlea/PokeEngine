#pragma pack_matrix(row_major) // 使用行矩阵

uint numTextures;
Texture2D texDiffuse : register(t0);
SamplerState samplerLinear : register(s0);
matrix localTransform;

cbuffer bufferMatrix : register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProjection;
};

struct VS_INPUT
{
	float4 pos:POSITION;
	float4 color:COLOR0;
	float2 tex:TEXCOORD0;
	float4 normal:NORMAL0;
};

struct PS_INPUT
{
	float4 pos:SV_POSITION;
	float4 color:COLOR0;
	float2 tex:TEXCOORD0;
	float4 normal:NORMAL0;
};

PS_INPUT vs_main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	//output.pos = mul(input.pos, localTransform);
	//output.pos = mul(output.pos, matWorld);
	//output.pos = mul(output.pos, matView);
	//output.pos = mul(output.pos, matProjection);
	matrix globalTransform = mul(localTransform, mul(matWorld, mul(matView, matProjection)));
	output.pos = mul(input.pos, globalTransform);
	output.tex = input.tex;
	output.color = input.color;
	output.normal = normalize(mul(input.normal, localTransform));

	return output;
}

float4 ps_main(PS_INPUT input):SV_Target
{
	// texture sample
	float4 texColor = float4(0, 0, 0, 0);
	// 有贴图
	if (numTextures > 0)
	{
		texColor = texDiffuse.Sample(samplerLinear, input.tex) * input.color;
	}
	// 无贴图
	else
	{
		texColor += input.color;
	}

	float4 light = normalize(float4(0, 1000, 0, 1));
	// 漫反射
	float4 diffuse = float4(0.5, 0.5, 0.5, 0.5) * dot(light, input.normal) * 0.5;

	float4 color = texColor + diffuse;
	return color;
}

technique11 MeshShader
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, vs_main()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ps_main()));
	}
}