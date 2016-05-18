#pragma once
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

#include "SceneObject.h"
#include "ModelData.h"

PE_BEGIN

/**
 @brief 场景中的3D模型元素，继承自SceneObject
 */
class Model3D : public SceneObject
{
public:
	/**
	 @brief 创建Model3D对象
	 @param model 模型文件路径
	 @param texture 纹理文件路径
	 */
	static std::shared_ptr<Model3D> Create(const std::string &model, const std::string &texture = "");

	virtual ~Model3D();

	/**
	 @brief 更新模型数据
	 @param stepTime 每一帧增加的时间
	 */
	virtual void Update(float stepTime);
	/**
	 @brief Get render data
	 */
	std::shared_ptr<RenderData> GetRenderData() const { return mModelData->renderData; };
	/**
	 @brief Get texture path
	 */
	const std::string& GetTexturePath() const { return mTexturePath; };

	/**
	 @brief 播放动画。如果不使用end参数，则把动画固定在start帧上
	 @param start 开始tick(frame)
	 @param end 结束tick(frame)
	 */
	void Play(float start, float end = -1);

	/**
	 @brief 停止动画
	 */
	void Stop();

protected:
	Model3D();
	/**
	 @brief 更新模型
	 @param node 当前node
	 @param animIndex 要更新的动画的编号，一般模型只包含一个动画，默认为0即take001，-1即无动画
	 */
	void RecursiveNode(ModelNode *node, int animIndex = -1);
	Matrix44f CalculateAnimate(ModelNode *node, int channelIndex, std::shared_ptr<AnimationData> anim);
	/**
	 @brief 静态模型更新一次数据
	 */
	void InitStaticOnce();

protected:
	std::shared_ptr<ModelData> mModelData;	//模型数据
	std::string mTexturePath;
	bool mIsStatic; // 是否是静态模型

	float mCurrentTime; // 当前帧的时间
	float mLastTime; // 前一帧的时间
	float mStart; // 动画开始点
	float mEnd; // 动画结束点
	bool mIsPlay; // 是否播放动画

	std::vector<Matrix44f*> mVertexGlobalTransform; // 顶点transform
};

PE_END

#endif