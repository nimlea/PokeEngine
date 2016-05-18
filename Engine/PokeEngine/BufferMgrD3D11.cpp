#include "BufferMgrD3D11.h"
#include "..\..\Libs\DirectXTK\Inc\WICTextureLoader.h"
#include "LogSystem.h"

// 检查buffer是否存在
#define ASSERT_BUFFER(buffer) { if (buffer < 0) return; }

PE_BEGIN

BufferMgrD3D11::BufferMgrD3D11()
{}

BufferMgrD3D11::~BufferMgrD3D11()
{}

void BufferMgrD3D11::Init(ID3D11Device *device, ID3D11DeviceContext *context)
{
	mDevice = device;
	mContext = context;
}

/*void BufferMgrD3D11::CreateTextureResource(const char *path)
{
	// check
	if (0 == strcmp(path, "")){ // 无纹理
		return;
	}
	if (mTextureResource.find(path) != mTextureResource.end()){ // 已创建资源
		return;
	}
	// 先把char转成wchar_t
	int w_size = MultiByteToWideChar(CP_ACP, 0, path, -1, nullptr, 0);
	wchar_t *w_path = new wchar_t[w_size];
	ZeroMemory(w_path, w_size);
	MultiByteToWideChar(CP_ACP, 0, path, -1, w_path, w_size);
	// 用DX Tool kit读取纹理
	ID3D11ShaderResourceView *view;
	HRESULT hr = CreateWICTextureFromFile(mDevice, mContext, w_path, nullptr, &view);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建shader resource view出错：%s", path);
		delete[] w_path;
	}
	else
	{
		mTextureResource[path] = view;
		delete[] w_path;
	}
}*/

int BufferMgrD3D11::CreateVertexBuffer(const void *data, unsigned int size)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA initData;
	ID3D11Buffer *buff;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;
	HRESULT hr = mDevice->CreateBuffer(&desc, &initData, &buff);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建dynamic vertex buff出错");
		return -1;
	}
	else
	{
		mVertexBuffer.push_back(buff);
		return (mVertexBuffer.size() - 1);
	}
}

int BufferMgrD3D11::CreateIndexBuffer(const void *data, unsigned int size)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA initData;
	ID3D11Buffer *buff;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;
	HRESULT hr = mDevice->CreateBuffer(&desc, &initData, &buff);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建dynamic index buff出错");
		return -1;
	}
	else
	{
		mIndexBuffer.push_back(buff);
		return (mVertexBuffer.size() - 1);
	}
}

void BufferMgrD3D11::UpdateVertexBuffer(int buffer, const void *data, unsigned int size)
{
	ASSERT_BUFFER(buffer);
	ID3D11Buffer *resource = mVertexBuffer[buffer];
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	mContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, data, size);
	mContext->Unmap(resource, 0);
}

void BufferMgrD3D11::UpdateIndexBuffer(int buffer, const void *data, unsigned int size)
{
	ASSERT_BUFFER(buffer);
	ID3D11Buffer *resource = mIndexBuffer[buffer];
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	mContext->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, data, size);
	mContext->Unmap(resource, 0);
}

void BufferMgrD3D11::SetVertexBuffer(int buffer, const unsigned int *stride)
{
	ASSERT_BUFFER(buffer);
	unsigned int offset = 0;
	mContext->IASetVertexBuffers(0, 1, &(mVertexBuffer[buffer]), stride, &offset);
}

void BufferMgrD3D11::SetIndexBuffer(int buffer)
{
	ASSERT_BUFFER(buffer);
	mContext->IASetIndexBuffer(mIndexBuffer[buffer], DXGI_FORMAT_R32_UINT, 0);
}

/*void BufferMgrD3D11::SetTextureResource(const char *path)
{
	if (mTextureResource.find(path) != mTextureResource.end())
	{
		ID3D11ShaderResourceView *view = mTextureResource[path];
		mContext->PSSetShaderResources(0, 1, &view);
	}
	else
	{
		mContext->PSSetShaderResources(0, 0, nullptr);
	}
}*/

PE_END