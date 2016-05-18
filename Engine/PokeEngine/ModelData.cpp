#include "ModelData.h"

#pragma warning(disable:4018)

PE_BEGIN

ModelData::~ModelData()
{
	ReleaseNode(rootNode);
	meshes.clear();
}

void ModelData::ReleaseNode(ModelNode *node)
{
	if (node != nullptr)
	{
		// 释放子节点内存
		for (int i = 0; i < node->children.size(); i++)
		{
			ReleaseNode(node->children[i]);
		}
		node->children.clear();
		// 释放本节点
		delete node;
		node = nullptr;
	}
}

PE_END