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
	 @brief ����effect
	 @param path �ļ�·��
	 @return ���ش�����effect��������ʧ���򷵻�-1
	 */
	int CreateEffect(const char *path);
	/**
	 @brief ����������
	 @return ���ش�����������ʧ�ܷ���-1
	 */
	int CreateSampler();

	/**
	 @brief ����VS��layout
	 @param effect effect��index
	 @param techName technique������
	 @param passName pass������
	 @param inputLayoutDesc VS��layout��������
	 @param numLayoutElemnets D3D11_INPUT_ELEMENT_DESC�����鳤��
	 */
	void SetInputLayout(int effectID, const char *techName, const char *passName, const D3D11_INPUT_ELEMENT_DESC *inputLayoutDesc, unsigned int numLayoutElements);

	/**
	 @brief ����int/uint����
	 @param effectID effect��index
	 @param name Ҫ���µı���������
	 @param data �µ�����
	 */
	void UpdateInt(int effectID, const char *name, int data);
	/**
	 @brief ����effect��ľ�������
	 @param effectID effect��index
	 @param name Ҫ���µı���������
	 @param data �µ�����
	 */
	void UpdateMatrix(int effectID, const char *name, const void *data);
	/**
	 @brief ����texture2d
	 @param effectID effect��index
	 @param name Ҫ���µı���������
	 @param path texture��·��
	 */
	void UpdateTexture2D(int effectID, const char *name, const char *path);
	
	/**
	 @brief ����effect���texture sampler
	 @param effectID effect��index
	 @param samplerName ���µ�sampler������
	 @param samplerID �µ�sampler��index
	 @param slot �Ĵ������
	 */
	void ApplySampler(int effectID, const char *samplerName, int samplerID, int slot);
	/**
	 @brief ������Ӧ��layout
	 */
	void ApplyLayout(int effectID, const char *techName, const char *passName);
	/**
	 @brief ����effect
	 @param effect effect��index
	 @param techName technique������
	 @param passName pass������
	 */
	void ApplyEffect(int effectID, const char *techName, const char *passName);

private:
	/**
	 @brief ����texture2D��Դ
	 @param path_key texture��·������Ϊmap��key
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