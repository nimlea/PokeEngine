#pragma once
#ifndef _EFFECTSYSTEMD3D11_H_
#define _EFFECTSYSTEMD3D11_H_

#include "..\Common\PEMacro.h"
#include <vector>
#include <unordered_map>
#include "..\..\Libs\d3dx11effect\Inc\d3dx11effect.h"

PE_BEGIN

class EffectSystemD3D11
{
public:
	EffectSystemD3D11();
	~EffectSystemD3D11();

	void Init(ID3D11Device *device, ID3D11DeviceContext *context);
	/**
	 @brief 创建effect
	 @param path 文件路径
	 @return 返回创建的effect的索引，失败则返回-1
	 */
	int CreateEffect(const char *path);
	/**
	 @brief 创建采样器
	 @return 返回创建的索引，失败返回-1
	 */
	int CreateSampler();

	/**
	 @brief 设置VS的layout
	 @param effect effect的index
	 @param techName technique的名字
	 @param passName pass的名字
	 @param inputLayoutDesc VS的layout描述数组
	 @param numLayoutElemnets D3D11_INPUT_ELEMENT_DESC的数组长度
	 */
	void SetInputLayout(int effectID, const char *techName, const char *passName, const D3D11_INPUT_ELEMENT_DESC *inputLayoutDesc, unsigned int numLayoutElements);

	/**
	 @brief 更新int/uint数据
	 @param effectID effect的index
	 @param name 要更新的变量的名字
	 @param data 新的数据
	 */
	void UpdateInt(int effectID, const char *name, int data);
	/**
	 @brief 更新effect里的矩阵数据
	 @param effectID effect的index
	 @param name 要更新的变量的名字
	 @param data 新的数据
	 */
	void UpdateMatrix(int effectID, const char *name, const void *data);
	/**
	 @brief 更新texture2d
	 @param effectID effect的index
	 @param name 要更新的变量的名字
	 @param path texture的路径
	 */
	void UpdateTexture2D(int effectID, const char *name, const char *path);
	
	/**
	 @brief 更新effect里的texture sampler
	 @param effectID effect的index
	 @param samplerName 更新的sampler的名字
	 @param samplerID 新的sampler的index
	 @param slot 寄存器编号
	 */
	void ApplySampler(int effectID, const char *samplerName, int samplerID, int slot);
	/**
	 @brief 启用相应的layout
	 */
	void ApplyLayout(int effectID, const char *techName, const char *passName);
	/**
	 @brief 启用effect
	 @param effect effect的index
	 @param techName technique的名字
	 @param passName pass的名字
	 */
	void ApplyEffect(int effectID, const char *techName, const char *passName);

private:
	/**
	 @brief 创建texture2D资源
	 @param path_key texture的路径，作为map的key
	 */
	void CreateTexture2D(const std::string &path_key);

private:
	ID3D11Device *mDevice;
	ID3D11DeviceContext *mContext;
	std::vector<ID3DX11Effect*> mEffects;

	std::unordered_map<std::string, ID3D11InputLayout*> mLayouts;
	std::vector<ID3D11SamplerState*> mSamplers;
	std::unordered_map<std::string, ID3D11ShaderResourceView*> mTexture2Ds;
};

PE_END

#endif