#include "ShaderMgrD3D11.h"
#include <d3dcompiler.h>
#include "LogSystem.h"

// 检查数据是否存在
#define ASSERT_SHADER(i) { if (i < 0) return; }

PE_BEGIN

ShaderMgrD3D11::ShaderMgrD3D11()
{}

ShaderMgrD3D11::~ShaderMgrD3D11()
{}

void ShaderMgrD3D11::Init(ID3D11Device *device, ID3D11DeviceContext *context)
{
	mDevice = device;
	mContext = context;
}

int ShaderMgrD3D11::CreateVS(const char *path, const D3D11_INPUT_ELEMENT_DESC *inputElementDesc, unsigned int numInputElement, const char *entry /* = "vs_main" */)
{
	ID3D11VertexShader *vs = nullptr;
	ID3DBlob *vsBlob = nullptr;

	// 先把char转成wchar_t
	int wchar_size = MultiByteToWideChar(CP_ACP, 0, path, -1, nullptr, 0);
	wchar_t *wchar_path = new wchar_t[wchar_size];
	ZeroMemory(wchar_path, wchar_size);
	MultiByteToWideChar(CP_ACP, 0, path, -1, wchar_path, wchar_size);

	if (0 == CompileShader(wchar_path, entry, "vs_5_0", &vsBlob)){
		return -1;
	}
	HRESULT hr = mDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建VS失败：%s", path);
		return -1;
	}

	// create vs input layout
	ID3D11InputLayout *layout;
	hr = mDevice->CreateInputLayout(inputElementDesc, numInputElement, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &layout);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建input layout失败：%s", path);
		return 0;
	}

	mVS.push_back(vs);
	mInputLayout.push_back(layout);
	vsBlob->Release();
	vsBlob = nullptr;
	delete[] wchar_path;
	return (mVS.size() - 1);
}

int ShaderMgrD3D11::CreatePS(const char *path, const char *entry /* = "ps_main" */)
{
	ID3D11PixelShader *ps = nullptr;
	ID3DBlob *psBlob = nullptr;

	// 先把char转成wchar_t
	int wchar_size = MultiByteToWideChar(CP_ACP, 0, path, -1, nullptr, 0);
	wchar_t *wchar_path = new wchar_t[wchar_size];
	ZeroMemory(wchar_path, wchar_size);
	MultiByteToWideChar(CP_ACP, 0, path, -1, wchar_path, wchar_size);

	if (0 == CompileShader(wchar_path, entry, "ps_5_0", &psBlob)){
		return -1;
	}
	HRESULT hr = mDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建PS失败：%s", path);
		return -1;
	}

	mPS.push_back(ps);
	psBlob->Release();
	psBlob = nullptr;
	delete[] wchar_path;
	return (mPS.size() - 1);
}

int ShaderMgrD3D11::CompileShader(const wchar_t *path, const char *entryPoint, const char *shaderModel, ID3DBlob **d3dBlob_out)
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef PE_DEBUG_MODE
	flags |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob *errorBlob = NULL;
	HRESULT hr = D3DCompileFromFile(path, NULL, NULL, entryPoint, shaderModel, flags, 0, d3dBlob_out, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob != NULL){
			LogSystem::GetInstance().Log("编译%ls出错， D3D Error：%s", path, (char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		return 0;
	}
	else
	{
		if (errorBlob != NULL){
			errorBlob->Release();
		}
		return 1;
	}
}

int ShaderMgrD3D11::CreateBuffer(unsigned int size)
{
	D3D11_BUFFER_DESC buffDesc;
	ZeroMemory(&buffDesc, sizeof(buffDesc));
	buffDesc.ByteWidth = size;
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	ID3D11Buffer *buffer;
	HRESULT hr = mDevice->CreateBuffer(&buffDesc, nullptr, &buffer);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建default constant buffer失败，HRESULT code：0x%x", hr);
		return -1;
	}

	mBuffer.push_back(buffer);
	return (mBuffer.size() - 1);
}

int ShaderMgrD3D11::CreateSampler()
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

	mSampler.push_back(sampler);
	return (mSampler.size() - 1);
}

void ShaderMgrD3D11::UpdateBuffer(int buffer, const void *data)
{
	mContext->UpdateSubresource(mBuffer[buffer], 0, nullptr, data, 0, 0);
}

void ShaderMgrD3D11::SetBufferBoth(int buffer, unsigned int slot)
{
	ASSERT_SHADER(buffer);
	mContext->VSSetConstantBuffers(slot, 1, &(mBuffer[buffer]));
	mContext->PSSetConstantBuffers(slot, 1, &(mBuffer[buffer]));
}

void ShaderMgrD3D11::SetSampler(int sampler, unsigned int slot)
{
	ASSERT_SHADER(sampler);
	mContext->PSSetSamplers(slot, 1, &(mSampler[sampler]));
}

void ShaderMgrD3D11::ApplyVS(int vs)
{
	ASSERT_SHADER(vs);
	mContext->VSSetShader(mVS[vs], nullptr, 0);
	mContext->IASetInputLayout(mInputLayout[vs]);
}

void ShaderMgrD3D11::ApplyPS(int ps)
{
	ASSERT_SHADER(ps);
	mContext->PSSetShader(mPS[ps], nullptr, 0);
}

PE_END