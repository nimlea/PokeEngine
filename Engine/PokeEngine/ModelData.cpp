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
		// �ͷ��ӽڵ��ڴ�
		for (int i = 0; i < node->children.size(); i++)
		{
			ReleaseNode(node->children[i]);
		}
		node->children.clear();
		// �ͷű��ڵ�
		delete node;
		node = nullptr;
	}
}

PE_END