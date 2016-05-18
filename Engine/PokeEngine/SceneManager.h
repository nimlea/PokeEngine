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
	 @brief 初始化
	 */
	void Init(float aspectRatio);

	/**
	 @brief 更新场景
	 @param stepTime 步进时间，一般为每帧时间，单位second
	 */
	void Update(float stepTime);

	/**
	 @brief 添加模型对象
	 */
	void AddModel3D(std::shared_ptr<Model3D> obj);

	/**
	 @brief 设置主摄像机
	 */
	void SetMainCamera(std::shared_ptr<Camera> camera);

	/**
	 @brief 设置最近渲染距离
	 */
	void SetNear(float near);

	/**
	 @brief 设置最远渲染距离
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
	float mAspectRatio; // 窗口比例
	float mNear;
	float mFar;

	std::shared_ptr<Camera> mMainCamera;
};

PE_END

#endif