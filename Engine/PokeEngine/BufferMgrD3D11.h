#pragma once
#ifndef _BUFFERMGRD3D11_H_
#define _BUFFERMGRD3D11_H_

#include "Model3D.h"
#include <d3d11.h>
#include <unordered_map>

PE_BEGIN

class BufferMgrD3D11
{
public:
	BufferMgrD3D11();
	~BufferMgrD3D11();

	void Init(ID3D11Device *device, ID3D11DeviceContext *context);

	/**
	 @brief 创建用于渲染的texture资源
	 @path 图片路径，也作为使用时的key
	 */
	//void CreateTextureResource(const char *path);
	/**
	 @brief 创建动态VertexBuffer
	 @param data 顶点数据
	 @param size 顶点总大小
	 @return 返回索引，失败返回-1
	 */
	int CreateVertexBuffer(const void *data, unsigned int size);
	/**
	 @brief 创建动态IndexBuffer
	 @param data 顶点索引数据
	 @param size 顶点索引总大小
	 @return 返回索引，失败返回-1
	 */
	int CreateIndexBuffer(const void *data, unsigned int size);

	//**************************************************************************************

	/**
	 @brief 更新VertexBuffer
	 */
	void UpdateVertexBuffer(int buffer, const void *data, unsigned int size);
	/**
	 @brief 更新IndexBuffer
	 */
	void UpdateIndexBuffer(int buffer, const void *data, unsigned int size);

	//**************************************************************************************

	/**
	 @brief 设置单个VertexBuffer
	 @param buffer 设置的VertexBuffer
	 @param stride 单个VertexBuffer数据结构的大小
	 */
	void SetVertexBuffer(int buffer, const unsigned int *stride);
	/**
	 @brief 设置IndexBuffer
	 */
	void SetIndexBuffer(int buffer);
	/**
	 @brief 设置纹理
	 @param path 纹理key
	 */
	//void SetTextureResource(const char *path);

private:
	ID3D11Device *mDevice;
	ID3D11DeviceContext *mContext;
	/**
	 防止图片资源重复创建，所以使用map
	 key - 图片路径，value - DX11 ResourceView
	 */
	std::unordered_map<const char*, ID3D11ShaderResourceView*> mTextureResource;
	std::vector<ID3D11Buffer*> mVertexBuffer;
	std::vector<ID3D11Buffer*> mIndexBuffer;
};

PE_END

#endif