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
	 @brief ����������Ⱦ��texture��Դ
	 @path ͼƬ·����Ҳ��Ϊʹ��ʱ��key
	 */
	//void CreateTextureResource(const char *path);
	/**
	 @brief ������̬VertexBuffer
	 @param data ��������
	 @param size �����ܴ�С
	 @return ����������ʧ�ܷ���-1
	 */
	int CreateVertexBuffer(const void *data, unsigned int size);
	/**
	 @brief ������̬IndexBuffer
	 @param data ������������
	 @param size ���������ܴ�С
	 @return ����������ʧ�ܷ���-1
	 */
	int CreateIndexBuffer(const void *data, unsigned int size);

	//**************************************************************************************

	/**
	 @brief ����VertexBuffer
	 */
	void UpdateVertexBuffer(int buffer, const void *data, unsigned int size);
	/**
	 @brief ����IndexBuffer
	 */
	void UpdateIndexBuffer(int buffer, const void *data, unsigned int size);

	//**************************************************************************************

	/**
	 @brief ���õ���VertexBuffer
	 @param buffer ���õ�VertexBuffer
	 @param stride ����VertexBuffer���ݽṹ�Ĵ�С
	 */
	void SetVertexBuffer(int buffer, const unsigned int *stride);
	/**
	 @brief ����IndexBuffer
	 */
	void SetIndexBuffer(int buffer);
	/**
	 @brief ��������
	 @param path ����key
	 */
	//void SetTextureResource(const char *path);

private:
	ID3D11Device *mDevice;
	ID3D11DeviceContext *mContext;
	/**
	 ��ֹͼƬ��Դ�ظ�����������ʹ��map
	 key - ͼƬ·����value - DX11 ResourceView
	 */
	std::unordered_map<const char*, ID3D11ShaderResourceView*> mTextureResource;
	std::vector<ID3D11Buffer*> mVertexBuffer;
	std::vector<ID3D11Buffer*> mIndexBuffer;
};

PE_END

#endif