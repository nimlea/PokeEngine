#include "EffectSystemD3D11.h"
#include <d3dcompiler.h>
#include "LogSystem.h"
#include "..\..\Libs\DirectXTK\Inc\WICTextureLoader.h"

#pragma warning(disable:4018)

#define ASSERT_EFFECT(i) { if (i < 0) return; }

PE_BEGIN

EffectSystemD3D11::EffectSystemD3D11()
{}

EffectSystemD3D11::~EffectSystemD3D11()
{
	for (int i = 0; i < mEffects.size(); i++)
	{
		mEffects[i]->Release();
		mEffects[i] = nullptr;
	}
	mEffects.clear();
}

void EffectSystemD3D11::Init(ID3D11Device *device, ID3D11DeviceContext *context)
{
	mDevice = device;
	mContext = context;
}

int EffectSystemD3D11::CreateEffect(const char *path)
{
	// 先把char转成wchar_t
	int wchar_size = MultiByteToWideChar(CP_ACP, 0, path, -1, nullptr, 0);
	wchar_t *wchar_path = new wchar_t[wchar_size];
	ZeroMemory(wchar_path, wchar_size);
	MultiByteToWideChar(CP_ACP, 0, path, -1, wchar_path, wchar_size);

	// compile
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef PE_DEBUG_MODE
	flags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob *effectBlob = nullptr;
	ID3DBlob *errorBlob = nullptr;
	// dx11只能使用fx_5_0
	HRESULT hr = D3DCompileFromFile(wchar_path, nullptr, nullptr, nullptr, "fx_5_0", flags, 0, &effectBlob, &errorBlob);	
	if (FAILED(hr))
	{
		if (errorBlob != nullptr){
			LogSystem::GetInstance().Log("编译%s出错， D3D Error：%s", path, (char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		return -1;
	}
	if (errorBlob != nullptr){
		errorBlob->Release();
	}

	// create effect
	ID3DX11Effect *effect;
	hr = D3DX11CreateEffectFromMemory(effectBlob->GetBufferPointer(), effectBlob->GetBufferSize(), 0, mDevice, &effect);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建%s出错", path);
		return -1;
	}

	mEffects.push_back(effect);
	return (mEffects.size() - 1);
}

void EffectSystemD3D11::SetInputLayout(int effectID, const char *techName, const char *passName, const D3D11_INPUT_ELEMENT_DESC *inputLayoutDesc, unsigned int numLayoutElements)
{
	ID3DX11Effect *effect = mEffects[effectID];
	D3DX11_PASS_DESC passDesc;
	effect->GetTechniqueByName(techName)->GetPassByName(passName)->GetDesc(&passDesc);

	ID3D11InputLayout *layout;
	HRESULT hr = mDevice->CreateInputLayout(inputLayoutDesc, numLayoutElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &layout);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建tech「%s」 - pass「%s」input layout失败", techName, passName);
		return;
	}

	std::string key(techName);
	key += passName;
	mLayouts[key] = layout;
}

int EffectSystemD3D11::CreateSampler()
{
	ID3D11SamplerState *sampler;
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = mDevice->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建default sampler state失败");
		return -1;
	}

	mSamplers.push_back(sampler);
	return (mSamplers.size() - 1);
}

void EffectSystemD3D11::UpdateMatrix(int effectID, const char *name, const void *data)
{
	ASSERT_EFFECT(effectID);
	ID3DX11EffectMatrixVariable *variable = nullptr;
	variable = mEffects[effectID]->GetVariableByName(name)->AsMatrix();
	if (variable->IsValid())
	{
		variable->SetMatrix((float*)data);
	}
	else
	{
		LogSystem::GetInstance().Log("effect matrix「%s」非法", name);
		return;
	}
}

void EffectSystemD3D11::ApplySampler(int effectID, const char *samplerName, int samplerID, int slot)
{
	ASSERT_EFFECT(effectID);
	ID3DX11EffectSamplerVariable *variable = mEffects[effectID]->GetVariableByName(samplerName)->AsSampler();
	ID3D11SamplerState *sampler = mSamplers[samplerID];
	variable->SetSampler(slot, sampler);
}

void EffectSystemD3D11::ApplyLayout(int effectID, const char *techName, const char *passName)
{
	ASSERT_EFFECT(effectID);
	std::string key(techName);
	key += passName;
	if (mLayouts.find(key) != mLayouts.end())
	{
		ID3D11InputLayout *layout = mLayouts[key];
		mContext->IASetInputLayout(layout);
	}
}

void EffectSystemD3D11::ApplyEffect(int effectID, const char *techName, const char *passName)
{
	ASSERT_EFFECT(effectID);
	ID3DX11EffectPass *pass = nullptr;
	pass = mEffects[effectID]->GetTechniqueByName(techName)->GetPassByName(passName);
	if (pass->IsValid())
	{
		pass->Apply(0, mContext);
	}
}

void EffectSystemD3D11::UpdateTexture2D(int effectID, const char *name, const char *path)
{
	ASSERT_EFFECT(effectID);
	std::string key(path);
	// 检查是否已创建
	if (mTexture2Ds.find(key) == mTexture2Ds.end())
	{
		CreateTexture2D(key);
	}
	// set resource
	ID3D11ShaderResourceView *resource = mTexture2Ds[key];
	ID3DX11EffectShaderResourceVariable *resourceVariable = mEffects[effectID]->GetVariableByName(name)->AsShaderResource();
	if (resourceVariable->IsValid())
	{
		resourceVariable->SetResource(resource);
	}
	else
	{
		LogSystem::GetInstance().Log("effect shader resource「%s」非法", name);
		return;
	}
}

void EffectSystemD3D11::CreateTexture2D(const std::string &path_key)
{
	// check
	if (0 == strcmp(path_key.c_str(), "")){ // 无纹理
		return;
	}
	if (mTexture2Ds.find(path_key) != mTexture2Ds.end()){ // 已创建资源
		return;
	}
	// 先把char转成wchar_t
	int w_size = MultiByteToWideChar(CP_ACP, 0, path_key.c_str(), -1, nullptr, 0);
	wchar_t *w_path = new wchar_t[w_size];
	ZeroMemory(w_path, w_size);
	MultiByteToWideChar(CP_ACP, 0, path_key.c_str(), -1, w_path, w_size);
	// 用DX Tool kit读取纹理
	ID3D11ShaderResourceView *view;
	HRESULT hr = DirectX::CreateWICTextureFromFile(mDevice, mContext, w_path, nullptr, &view);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建shader resource view出错：%s", path_key);
	}
	else
	{
		mTexture2Ds[path_key] = view;
	}
	delete[] w_path;
}

void EffectSystemD3D11::UpdateInt(int effectID, const char *name, int data)
{
	ID3DX11EffectScalarVariable *integerVariable = mEffects[effectID]->GetVariableByName(name)->AsScalar();
	if (integerVariable->IsValid())
	{
		integerVariable->SetInt(data);
	}
	else
	{
		LogSystem::GetInstance().Log("effect integer「%s」非法", name);
		return;
	}
}

PE_END