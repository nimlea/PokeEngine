#pragma once
#ifndef _SHADERMGRD3D11_H_
#define _SHADERMGRD3D11_H_

#include "..\Common\PEMacro.h"
#include <d3d11.h>
#include <vector>

PE_BEGIN

class ShaderMgrD3D11
{
public:
	ShaderMgrD3D11();
	~ShaderMgrD3D11();

	void Init(ID3D11Device *device, ID3D11DeviceContext *context);

	/**
	 @brief ��Cache�д���vertex shader
	 @param path �ļ�·��
	 @param inputElementDesc ��ں����Ĳ�����������
	 @param numInputElement D3D11_INPUT_ELEMENT_DESC����ĳ���
	 @param entry ��ں�������
	 @return ���ش�����vertex shader��Cache�����������ʧ���򷵻�-1
	 */
	int CreateVS(const char *path, const D3D11_INPUT_ELEMENT_DESC *inputElementDesc, unsigned int numInputElement, const char *entry = "vs_main");
	/**
	 @brief ��Cache�д���pixel shader
	 @return ���ش�����pixel shader��Cache�����������ʧ���򷵻�-1
	 */
	int CreatePS(const char *path, const char *entry = "ps_main");
	/**
	 @brief ����shader���buffer
	 @param size buffer�Ĵ�С
	 @return ����Cache�е�������ʧ�ܷ���-1
	 */
	int CreateBuffer(unsigned int size);
	/**
	 @brief ����������
	 @return ����Cache�е�������ʧ�ܷ���-1
	 */
	int CreateSampler();

	//**************************************************************************************

	/**
	 @brief ����shader��buffer
	 @param buffer ��Ҫ���µ�buffer
	 @param data ���µ�����Դ
	 */
	void UpdateBuffer(int buffer, const void *data);

	//**************************************************************************************

	/**
	 @brief ��VS��PS�����õ���buffer
	 @param buffer ���õ�buffer
	 @param slot �Ĵ���
	 */
	void SetBufferBoth(int buffer, unsigned int slot);
	/**
	 @brief ����PS�еĲ�����
	 @param sampler ���õĲ�����
	 @param slot �Ĵ���
	 */
	void SetSampler(int sampler, unsigned int slot);
	/**
	 @brief Ӧ��VS
	 */
	void ApplyVS(int vs);
	/**
	 @brief Ӧ��PS
	 */
	void ApplyPS(int ps);

private:
	int CompileShader(const wchar_t *path, const char *entryPoint, const char *shaderModel, ID3DBlob **d3dBlob_out);

private:
	ID3D11Device *mDevice;
	ID3D11DeviceContext *mContext;
	std::vector<ID3D11VertexShader*> mVS;
	std::vector<ID3D11InputLayout*> mInputLayout; // ��vsʹ��ͬһ����
	std::vector<ID3D11PixelShader*> mPS;
	std::vector<ID3D11SamplerState*> mSampler;
	std::vector<ID3D11Buffer*> mBuffer;
};

PE_END

#endif