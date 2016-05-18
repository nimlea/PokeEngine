#pragma once
#ifndef _LOADERASSIMP_H_
#define _LOADERASSIMP_H_

#include "ModelData.h"
#include "..\..\Libs\assimp\include\scene.h"

PE_BEGIN

class LoaderAssimp
{
public:
	static std::shared_ptr<ModelData> Load(const std::string path);

private:
	// ת��aiScene����
	static std::shared_ptr<ModelData> ProcessSceneData(const aiScene *scene);
	// �ݹ鴦��aiScene���node
	static ModelNode* RecursiveNodeData(const aiNode *assimpNode);
	// ת��aiMesh����
	static std::shared_ptr<Meshdata> ProcessMeshData(const aiMesh *assimpMesh);
	/**
	 @brief ���������ɵ�ModelData���Node�����ݹ�ת��һ��aiMesh�ڵ�aiBone���ݣ��������������Ƿ��ظ�
	 @param node ��ǰ�ݹ鵽��node
	 @param assimpMesh �����meshȡ�ù�������
	 @param meshIndex �����󶨵�mesh��index
	 */
	static BoneData* RecursiveBoneData(const ModelNode *node, const aiMesh *assimpMesh, int meshIndex);
	// ת��aiAnimation����
	static std::shared_ptr<AnimationData> ProcessAnimationData(const aiAnimation *assimpAnim);
	/**
	 @brief ��ȡnode��AnimationData���Ӧ��channel index�����node�޶�����ֵ-1
	 @param node ��ǰ�ݹ鵽��node
	 @param animIndex �����ı��
	 @param animData ��������
	 */
	static void RecursiveAnimData(ModelNode *node, int animIndex, std::shared_ptr<AnimationData> animData);
	/**
	 @brief �ݹ��ʼ����Ⱦ��mesh
	 @param node ��ǰnode
	 @param model �������ݵ�ModelData
	 */
	static void RecursiveInitRenderMesh(ModelNode *node, std::shared_ptr<ModelData> model);

	// ��assimp�ľ����ʽת��Ϊ�Զ����ʽ
	static Matrix44f GetMatrix44(const aiMatrix4x4 &m);
	// ��assimp��������ʽתΪ�Զ����ʽ
	static Vector3f GetVector3(const aiVector3D &v);
	/**
	 @brief ��ʾ�������
	 @param percent ������ȵİٷֱ�
	 @param action ��ǰ��Ϊ
	 */
	static void Progress(float percent, const char *action);
};

PE_END

#endif