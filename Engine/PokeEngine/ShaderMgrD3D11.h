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
	 @brief 在Cache中创建vertex shader
	 @param path 文件路径
	 @param inputElementDesc 入口函数的参数数据类型
	 @param numInputElement D3D11_INPUT_ELEMENT_DESC数组的长度
	 @param entry 入口函数名称
	 @return 返回创建的vertex shader在Cache中索引，如果失败则返回-1
	 */
	int CreateVS(const char *path, const D3D11_INPUT_ELEMENT_DESC *inputElementDesc, unsigned int numInputElement, const char *entry = "vs_main");
	/**
	 @brief 在Cache中创建pixel shader
	 @return 返回创建的pixel shader在Cache中索引，如果失败则返回-1
	 */
	int CreatePS(const char *path, const char *entry = "ps_main");
	/**
	 @brief 创建shader里的buffer
	 @param size buffer的大小
	 @return 返回Cache中的索引，失败返回-1
	 */
	int CreateBuffer(unsigned int size);
	/**
	 @brief 创建采样器
	 @return 返回Cache中的索引，失败返回-1
	 */
	int CreateSampler();

	//**************************************************************************************

	/**
	 @brief 更新shader的buffer
	 @param buffer 需要更新的buffer
	 @param data 更新的数据源
	 */
	void UpdateBuffer(int buffer, const void *data);

	//**************************************************************************************

	/**
	 @brief 在VS和PS中设置单个buffer
	 @param buffer 设置的buffer
	 @param slot 寄存器
	 */
	void SetBufferBoth(int buffer, unsigned int slot);
	/**
	 @brief 设置PS中的采样器
	 @param sampler 设置的采样器
	 @param slot 寄存器
	 */
	void SetSampler(int sampler, unsigned int slot);
	/**
	 @brief 应用VS
	 */
	void ApplyVS(int vs);
	/**
	 @brief 应用PS
	 */
	void ApplyPS(int ps);

private:
	int CompileShader(const wchar_t *path, const char *entryPoint, const char *shaderModel, ID3DBlob **d3dBlob_out);

private:
	ID3D11Device *mDevice;
	ID3D11DeviceContext *mContext;
	std::vector<ID3D11VertexShader*> mVS;
	std::vector<ID3D11InputLayout*> mInputLayout; // 和vs使用同一索引
	std::vector<ID3D11PixelShader*> mPS;
	std::vector<ID3D11SamplerState*> mSampler;
	std::vector<ID3D11Buffer*> mBuffer;
};

PE_END

#endif