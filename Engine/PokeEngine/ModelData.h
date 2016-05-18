#pragma once
#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include <memory>
#include <vector>
#include "..\Common\PEMacro.h"
#include "..\..\Libs\libMath\Vector2.h"
#include "..\..\Libs\libMath\Vector3.h"
#include "..\..\Libs\libMath\Vector4.h"
#include "..\..\Libs\libMath\Matrix44.h"
#include "AnimationData.h"
#include "BoneData.h"

PE_BEGIN

#define MAX_UV_COUNT 1

// �������ݸ�ʽ
struct VertexData
{
	Vector3f pos;
	Vector4f color;
	Vector2f uv[MAX_UV_COUNT];
	Vector3f normal;
};

struct Meshdata
{
	VertexData *vertexData = nullptr;
	unsigned int *indexData = nullptr;

	unsigned int vertexCount = 0;
	unsigned int indexCount = 0;

	// only be used in ModelData::RenderData
	int vertexBufferID = -1;
	int indexBufferID = -1;

	~Meshdata()
	{
		if (vertexData != nullptr) delete[] vertexData;
		if (indexData != nullptr) delete[] indexData;
	}
};

enum E_ModelNodeType
{
	eMNT_NONE = 0,
	eMNT_MESH = 1,
	eMNT_BONE,
};

struct ModelNode
{
	E_ModelNodeType type = E_ModelNodeType::eMNT_NONE;
	char name[256];

	Matrix44f localTransform; // �ֲ�����
	Matrix44f globalTransform; // �������
	std::vector<unsigned int> modelMeshesID; // �ڵ������meshes��ʵ�ʵ�ԭʼ���ݴ�����ModelData���meshes�������֮��Ӧ��index
	std::vector<unsigned int> renderMeshesID; // meshes��RenderData

	BoneData *boneData = nullptr; // ���Ϊ�����ڵ㣬����ڹ�������
	std::vector<int> channelsID; // ����channel
	unsigned int lastPositionFrame = 0;
	unsigned int lastRotationFrame = 0;
	unsigned int lastScalingFrame = 0;

	ModelNode *parent = nullptr;
	std::vector<ModelNode*> children; // ��ModelData���ͷ��ڴ�ռ�

	ModelNode()
	{
		memset(name, '\0', sizeof(name));
	}
	~ModelNode()
	{
		if (boneData != nullptr) delete boneData;
	}
};

/**
 @brief ��Ⱦ������
 */
struct RenderData
{
	std::vector<std::shared_ptr<Meshdata>> meshes;
};

struct ModelData
{
	ModelNode *rootNode = nullptr;

	std::vector<std::shared_ptr<Meshdata>> meshes;
	std::vector<std::shared_ptr<AnimationData>> animations;

	std::shared_ptr<RenderData> renderData;

	~ModelData();
	void ReleaseNode(ModelNode *node);
};

PE_END

#endif