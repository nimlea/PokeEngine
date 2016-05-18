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
	// 转换aiScene数据
	static std::shared_ptr<ModelData> ProcessSceneData(const aiScene *scene);
	// 递归处理aiScene里的node
	static ModelNode* RecursiveNodeData(const aiNode *assimpNode);
	// 转换aiMesh数据
	static std::shared_ptr<Meshdata> ProcessMeshData(const aiMesh *assimpMesh);
	/**
	 @brief 根据已生成的ModelData里的Node树，递归转换一个aiMesh内的aiBone数据，不检测骨骼名字是否重复
	 @param node 当前递归到的node
	 @param assimpMesh 从这个mesh取得骨骼数据
	 @param meshIndex 骨骼绑定的mesh的index
	 */
	static BoneData* RecursiveBoneData(const ModelNode *node, const aiMesh *assimpMesh, int meshIndex);
	// 转换aiAnimation数据
	static std::shared_ptr<AnimationData> ProcessAnimationData(const aiAnimation *assimpAnim);
	/**
	 @brief 获取node在AnimationData里对应的channel index，如果node无动画则赋值-1
	 @param node 当前递归到的node
	 @param animIndex 动画的编号
	 @param animData 动画数据
	 */
	static void RecursiveAnimData(ModelNode *node, int animIndex, std::shared_ptr<AnimationData> animData);
	/**
	 @brief 递归初始化渲染用mesh
	 @param node 当前node
	 @param model 生成数据的ModelData
	 */
	static void RecursiveInitRenderMesh(ModelNode *node, std::shared_ptr<ModelData> model);

	// 把assimp的矩阵格式转换为自定义格式
	static Matrix44f GetMatrix44(const aiMatrix4x4 &m);
	// 把assimp的向量格式转为自定义格式
	static Vector3f GetVector3(const aiVector3D &v);
	/**
	 @brief 显示载入进度
	 @param percent 载入进度的百分比
	 @param action 当前行为
	 */
	static void Progress(float percent, const char *action);
};

PE_END

#endif