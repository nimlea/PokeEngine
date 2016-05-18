#pragma once
#ifndef _MODEL3D_H_
#define _MODEL3D_H_

#include "SceneObject.h"
#include "ModelData.h"

PE_BEGIN

/**
 @brief �����е�3Dģ��Ԫ�أ��̳���SceneObject
 */
class Model3D : public SceneObject
{
public:
	/**
	 @brief ����Model3D����
	 @param model ģ���ļ�·��
	 @param texture �����ļ�·��
	 */
	static std::shared_ptr<Model3D> Create(const std::string &model, const std::string &texture = "");

	virtual ~Model3D();

	/**
	 @brief ����ģ������
	 @param stepTime ÿһ֡���ӵ�ʱ��
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
	 @brief ���Ŷ����������ʹ��end��������Ѷ����̶���start֡��
	 @param start ��ʼtick(frame)
	 @param end ����tick(frame)
	 */
	void Play(float start, float end = -1);

	/**
	 @brief ֹͣ����
	 */
	void Stop();

protected:
	Model3D();
	/**
	 @brief ����ģ��
	 @param node ��ǰnode
	 @param animIndex Ҫ���µĶ����ı�ţ�һ��ģ��ֻ����һ��������Ĭ��Ϊ0��take001��-1���޶���
	 */
	void RecursiveNode(ModelNode *node, int animIndex = -1);
	Matrix44f CalculateAnimate(ModelNode *node, int channelIndex, std::shared_ptr<AnimationData> anim);
	/**
	 @brief ��̬ģ�͸���һ������
	 */
	void InitStaticOnce();

protected:
	std::shared_ptr<ModelData> mModelData;	//ģ������
	std::string mTexturePath;
	bool mIsStatic; // �Ƿ��Ǿ�̬ģ��

	float mCurrentTime; // ��ǰ֡��ʱ��
	float mLastTime; // ǰһ֡��ʱ��
	float mStart; // ������ʼ��
	float mEnd; // ����������
	bool mIsPlay; // �Ƿ񲥷Ŷ���

	std::vector<Matrix44f*> mVertexGlobalTransform; // ����transform
};

PE_END

#endif