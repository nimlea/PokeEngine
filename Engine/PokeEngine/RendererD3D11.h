#pragma once
#ifndef _RENDERERD3D11_H_
#define _RENDERERD3D11_H_

#include "IRenderer.h"
#include <d3d11.h>
#include "ShaderMgrD3D11.h"
#include "EffectSystemD3D11.h"
#include "BufferMgrD3D11.h"

PE_BEGIN

class RendererD3D11 : public IRenderer
{
public:
	RendererD3D11();
	virtual ~RendererD3D11();

	virtual int Init(void *hWnd);
	virtual void Release();
	virtual void Render(const std::shared_ptr<SceneManager> sceneMgr);

private:
	/**
	 @brief 创建渲染的一些自带shader
	 */
	void CreateDefaultShader();
	/**
	 @brief 渲染Mesh
	 */
	void RenderMesh(const std::shared_ptr<SceneManager> sceneMgr);

private:
	void *mHWnd;

	ID3D11Device			*mDevice;
	ID3D11DeviceContext		*mContext; // immediate context,"represents a single stream of commands that go to the GPU" - by MSDN
	IDXGISwapChain			*mSwapChain;
	ID3D11RasterizerState	*mRasterState;
	ID3D11RenderTargetView	*mRenderTargetView;
	ID3D11Texture2D			*mDepthStencilResource;
	ID3D11DepthStencilView	*mDepthStencilView;
	D3D_DRIVER_TYPE			mDriverType;
	D3D_FEATURE_LEVEL		mFeatureLevel;

	// render settings
	D3D11_VIEWPORT mViewport;

	// default shader
	//std::shared_ptr<ShaderMgrD3D11> mShaderMgr;
	//int mMeshVS;
	//int mMeshPS;
	//int mSceneMatrix;
	std::shared_ptr<EffectSystemD3D11> mEffectSys;
	int mMeshFx;
	int mTextureSampler;

	std::shared_ptr<BufferMgrD3D11> mBufferMgr;
};

PE_END

#endif