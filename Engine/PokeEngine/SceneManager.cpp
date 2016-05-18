#include "SceneManager.h"
#include "..\..\Libs\libMath\MathUtility.h"

PE_BEGIN

SceneManager::SceneManager()
:mAspectRatio(1.0f), mNear(0.1f), mFar(1000.0f)
{}

SceneManager::~SceneManager()
{}

void SceneManager::Init(float aspectRatio)
{
	mSceneMatrix.reset(new SceneMatrix());

	// 初始化主摄像机
	mMainCamera.reset(new Camera());
	mMainCamera->SetPosition(0.0f, 0.0f, 1.0f);
	mMainCamera->SetLookat(0.0f, 0.0f, 0.0f);

	// scene matrix
	mAspectRatio = aspectRatio;
	mSceneMatrix->world.Indentity();
	mSceneMatrix->view = MatrixLookAtRH(mMainCamera->GetPosition(), mMainCamera->GetLookat(), mMainCamera->GetUp());
	mSceneMatrix->projection = MatrixPerspectiveProjectionRH(MATH_PI / 4, mAspectRatio, mNear, mFar);
}

void SceneManager::Update(float stepTime)
{
	for (unsigned int i = 0; i < mModelObjs.size(); i++)
	{
		mModelObjs[i]->Update(stepTime);
	}
}

void SceneManager::AddModel3D(std::shared_ptr<Model3D> obj)
{
	mModelObjs.push_back(obj);
}

void SceneManager::SetMainCamera(std::shared_ptr<Camera> camera)
{
	mMainCamera = camera;
	mSceneMatrix->view = MatrixLookAtRH(mMainCamera->GetPosition(), mMainCamera->GetLookat(), mMainCamera->GetUp());
}

void SceneManager::SetNear(float near)
{
	mNear = near;
	mSceneMatrix->projection = MatrixPerspectiveProjectionRH(MATH_PI / 4, mAspectRatio, mNear, mFar);
}

void SceneManager::SetFar(float far)
{
	mFar = far;
	mSceneMatrix->projection = MatrixPerspectiveProjectionRH(MATH_PI / 4, mAspectRatio, mNear, mFar);
}

PE_END