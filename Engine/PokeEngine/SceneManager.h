#pragma once
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <vector>
#include "SceneObject.h"
#include "Model3D.h"
#include "Camera.h"

PE_BEGIN

struct SceneMatrix
{
	Matrix44f world;
	Matrix44f view;
	Matrix44f projection;
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	/**
	 @brief ��ʼ��
	 */
	void Init(float aspectRatio);

	/**
	 @brief ���³���
	 @param stepTime ����ʱ�䣬һ��Ϊÿ֡ʱ�䣬��λsecond
	 */
	void Update(float stepTime);

	/**
	 @brief ���ģ�Ͷ���
	 */
	void AddModel3D(std::shared_ptr<Model3D> obj);

	/**
	 @brief �����������
	 */
	void SetMainCamera(std::shared_ptr<Camera> camera);

	/**
	 @brief ���������Ⱦ����
	 */
	void SetNear(float near);

	/**
	 @brief ������Զ��Ⱦ����
	 */
	void SetFar(float far);

	/**
	 @brief
	 */
	std::shared_ptr<SceneMatrix> GetSceneMatrix() const { return mSceneMatrix; };

	/**
	 @brief Get 3d model vector
	 */
	std::vector<std::shared_ptr<Model3D>> GetModels() const { return mModelObjs; };

	/**
	 @brief Get main camera
	 */
	std::shared_ptr<Camera> GetMainCamera() const { return mMainCamera; };
	
private:

private:
	std::vector<std::shared_ptr<Model3D>> mModelObjs;
	std::shared_ptr<SceneMatrix> mSceneMatrix;
	float mAspectRatio; // ���ڱ���
	float mNear;
	float mFar;

	std::shared_ptr<Camera> mMainCamera;
};

PE_END

#endif