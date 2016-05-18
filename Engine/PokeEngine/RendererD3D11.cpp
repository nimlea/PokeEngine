#include "RendererD3D11.h"
#include "LogSystem.h"
#include "..\..\Libs\libMath\MathUtility.h"

#pragma comment (lib, "D3D11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#pragma warning(disable:4018)

PE_BEGIN

RendererD3D11::RendererD3D11()
{
	mDriverType = D3D_DRIVER_TYPE_NULL;
	mFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// shaders
	//mMeshVS = -1;
	//mMeshPS = -1;
	//mSceneMatrix = -1;
	mMeshFx = -1;
	mTextureSampler = -1;
}

RendererD3D11::~RendererD3D11()
{
#ifdef PE_DEBUG_MODE
	LogSystem::GetInstance().Log("析构RendererD3D11");
#endif
	Release();
}

int RendererD3D11::Init(void *hWnd)
{
	mHWnd = hWnd;

	RECT rc;
	GetClientRect((HWND)mHWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	// 填充 swap chain 结构
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1; // back buffer —— by 《3D Game Programming With DirectX11》,seems some bugs on MSDN,ehnn..
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = (HWND)mHWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = true;

	UINT createDeviceFlags = 0;
#ifdef PE_DEBUG_MODE
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 设备类型
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// 创建设备
	HRESULT hr;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		mDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, mDriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &desc, &mSwapChain, &mDevice, &mFeatureLevel, &mContext);
		if (SUCCEEDED(hr)){
			LogSystem::GetInstance().Log("创建D3D设备成功");
			break;
		}
	}
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("初始化D3D设备失败");
		return 0;
	}

	// 设置光栅器选项
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = true; // 逆时针渲染顶点
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.AntialiasedLineEnable = false;
	hr = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (SUCCEEDED(hr))
	{
		mContext->RSSetState(mRasterState);
	}
	else
	{
		LogSystem::GetInstance().Log("创建光栅器状态失败");
	}


	// 创建render target view
	ID3D11Texture2D* backBuffer = NULL;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("SwapChain创建BackBuffer失败");
		return 0;
	}
	hr = mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
	backBuffer->Release();
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建RenderTargetView失败");
		return 0;
	}
	// 创建depth stencil view
	D3D11_TEXTURE2D_DESC dsTexDesc;
	dsTexDesc.Width = width;
	dsTexDesc.Height = height;
	dsTexDesc.MipLevels = 1;
	dsTexDesc.ArraySize = 1;
	dsTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsTexDesc.SampleDesc.Count = 1;
	dsTexDesc.SampleDesc.Quality = 0;
	dsTexDesc.Usage = D3D11_USAGE_DEFAULT;
	dsTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsTexDesc.CPUAccessFlags = 0;
	dsTexDesc.MiscFlags = 0;
	hr = mDevice->CreateTexture2D(&dsTexDesc, nullptr, &mDepthStencilResource);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建depth stencil resource失败");
		return 0;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc;
	ZeroMemory(&dsViewDesc, sizeof(dsViewDesc));
	dsViewDesc.Format = dsTexDesc.Format;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Texture2D.MipSlice = 0;
	hr = mDevice->CreateDepthStencilView(mDepthStencilResource, &dsViewDesc, &mDepthStencilView);
	if (FAILED(hr))
	{
		LogSystem::GetInstance().Log("创建depth stencil view失败");
		return 0;
	}
	// 设置render target
	mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// 设置视口
	mViewport.Width = (FLOAT)width;
	mViewport.Height = (FLOAT)height;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mContext->RSSetViewports(1, &mViewport);

	// 创建默认shader
	//mShaderMgr.reset(new ShaderMgrD3D11());
	//mShaderMgr->Init(mDevice, mContext);
	mEffectSys.reset(new EffectSystemD3D11());
	mEffectSys->Init(mDevice, mContext);
	CreateDefaultShader();

	// 渲染用buffer
	mBufferMgr.reset(new BufferMgrD3D11());
	mBufferMgr->Init(mDevice, mContext);

	return 1;
}

void RendererD3D11::Release()
{
	// 释放DX11相关
	if (mContext != nullptr)				mContext->ClearState();

	if (mRasterState != nullptr)			mRasterState->Release();
	if (mDepthStencilResource != nullptr)	mDepthStencilResource->Release();
	if (mRenderTargetView != nullptr)		mRenderTargetView->Release();
	if (mDepthStencilView != nullptr)		mDepthStencilView->Release();
	if (mSwapChain != nullptr)				mSwapChain->Release();
	if (mContext != nullptr)				mContext->Release();
	// Debug下输出资源释放情况
#ifdef PE_DEBUG_MODE
	ID3D11Debug *d3dDebug;
	HRESULT hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
	if (d3dDebug != nullptr)			d3dDebug->Release();
#endif
	if (mDevice != nullptr)				mDevice->Release();
}

void RendererD3D11::Render(const std::shared_ptr<SceneManager> sceneMgr)
{
	// 初始化一些渲染的东西
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mContext->ClearRenderTargetView(mRenderTargetView, ClearColor);

	// 测试代码，旋转
	//static float t = 0.0f;
	//static DWORD dwTimeStart = 0;
	//DWORD dwTimeCur = GetTickCount();
	//if (dwTimeStart == 0)
	//	dwTimeStart = dwTimeCur;
	//t = (dwTimeCur - dwTimeStart) / 1000.0f;
	//sceneMgr->GetSceneMatrix()->world = MatrixRotationY(t);

	RenderMesh(sceneMgr);

	mSwapChain->Present(0, 0);
}

void RendererD3D11::RenderMesh(const std::shared_ptr<SceneManager> sceneMgr)
{
	std::vector<std::shared_ptr<Model3D>> objs = sceneMgr->GetModels();
	// set shader variables & settings
	//mShaderMgr->UpdateBuffer(mSceneMatrix, sceneMgr->GetSceneMatrix().get());
	//mShaderMgr->SetBufferBoth(mSceneMatrix, 0);
	mEffectSys->UpdateMatrix(mMeshFx, "matWorld", &(sceneMgr->GetSceneMatrix()->world));
	mEffectSys->UpdateMatrix(mMeshFx, "matView", &(sceneMgr->GetSceneMatrix()->view));
	mEffectSys->UpdateMatrix(mMeshFx, "matProjection", &(sceneMgr->GetSceneMatrix()->projection));
	mEffectSys->ApplySampler(mMeshFx, "samplerLinear", mTextureSampler, 0);
	mEffectSys->ApplyLayout(mMeshFx, "MeshShader", "p0");

	mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// 设置图元类型
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 遍历model
	for (int i = 0; i < objs.size(); i++)
	{
		std::shared_ptr<Model3D> model = objs[i];
		// 遍历model内的mesh
		for (int j = 0; j < model->GetRenderData()->meshes.size(); j++ )
		{
			std::shared_ptr<Meshdata> mesh = model->GetRenderData()->meshes[j];
			// check vertex buffer
			if (mesh->vertexBufferID < 0)
			{
				mesh->vertexBufferID = mBufferMgr->CreateVertexBuffer(mesh->vertexData, sizeof(VertexData)* mesh->vertexCount);
			}
			mBufferMgr->UpdateVertexBuffer(mesh->vertexBufferID, mesh->vertexData, sizeof(VertexData)* mesh->vertexCount);
			// check index buffer
			if (mesh->indexBufferID < 0)
			{
				mesh->indexBufferID = mBufferMgr->CreateIndexBuffer(mesh->indexData, sizeof(unsigned int)* mesh->indexCount);
			}
			mBufferMgr->UpdateIndexBuffer(mesh->indexBufferID, mesh->indexData, sizeof(unsigned int)* mesh->indexCount);
			// check texture buffer
			//mBufferMgr->CreateTextureResource(model->GetTexture().c_str());

			// set buffer
			unsigned int stride = sizeof(VertexData);
			mBufferMgr->SetVertexBuffer(mesh->vertexBufferID, &stride);
			mBufferMgr->SetIndexBuffer(mesh->indexBufferID);
			// set texture
			//mBufferMgr->SetTextureResource(model->GetTexture().c_str());
			//mShaderMgr->SetSampler(mTextureSampler, 0);
			if (0 == strcmp("", model->GetTexturePath().c_str()))
			{
				// 无纹理贴图
				mEffectSys->UpdateInt(mMeshFx, "numTextures", 0);
			}
			else
			{
				// 有纹理贴图
				mEffectSys->UpdateInt(mMeshFx, "numTextures", 1);
				mEffectSys->UpdateTexture2D(mMeshFx, "texDiffuse", model->GetTexturePath().c_str());
			}
			// set transform
			mEffectSys->UpdateMatrix(mMeshFx, "localTransform", &(model->GetLocalTransform()));
			// apply shader
			//mShaderMgr->ApplyVS(mMeshVS);
			//mShaderMgr->ApplyPS(mMeshPS);
			mEffectSys->ApplyEffect(mMeshFx, "MeshShader", "p0");
			// 渲染
			mContext->DrawIndexed(mesh->indexCount, 0, 0);
		}
	}
}

void RendererD3D11::CreateDefaultShader()
{
	//mSceneMatrix = mShaderMgr->CreateBuffer(sizeof(SceneMatrix));

	D3D11_INPUT_ELEMENT_DESC mesh_layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//mMeshVS = mShaderMgr->CreateVS("Mesh.fx", mesh_input, ARRAYSIZE(mesh_input));
	//mMeshPS = mShaderMgr->CreatePS("Mesh.fx");
	//mTextureSampler = mShaderMgr->CreateSampler();

	mMeshFx = mEffectSys->CreateEffect("Mesh.fx");
	mEffectSys->SetInputLayout(mMeshFx, "MeshShader", "p0", mesh_layout, ARRAYSIZE(mesh_layout));
	mTextureSampler = mEffectSys->CreateSampler();
}

PE_END