#pragma once
#ifndef _SCENEOBJECT_H_
#define _SCENEOBJECT_H_

#include <memory>
#include "..\Common\PEMacro.h"
#include "..\..\Libs\libMath\Vector3.h"
#include "..\..\Libs\libMath\Matrix44.h"

PE_BEGIN

/**
@brief 更新前调用
@param self SceneObject对象本身的指针
*/
typedef void(*PreUpdateHandler)(void *self);
/**
@brief 更新完后调用
@param self SceneObject对象本身的指针
*/
typedef void(*PostUpdateHandler)(void *self);

/**
 @brief 作为场景中最基础元素的类，场景中的元素都继承自此类
 */
class SceneObject
{
public:
	SceneObject();
	virtual ~SceneObject();

	virtual void Enable() { mIsEnable = true; };
	virtual void Disable() { mIsEnable = false; };

	// ***************** transform *******************
	/**
	 @brief 设置位置，xyz值为直接值，非位移增量
	 */
	virtual void SetPosition(const Vector3f &p);
	/**
	@brief 设置位置，xyz值为直接值，非位移增量
	*/
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPositionX(float x);
	virtual void SetPositionY(float y);
	virtual void SetPositionZ(float z);
	/**
	 @brief 以模型最初的角度为基准旋转
	 @param angel 本次旋转角度
	 */
	virtual void SetRotationXByDegrees(float angel);
	virtual void SetRotationYByDegrees(float angel);
	virtual void SetRotationZByDegrees(float angel);
	/**
	 @brief 整体缩放
	 @param scale 缩放倍数
	 */
	virtual void SetScaling(float scale);
	/**
	 @brief 分别缩放各分量
	 */
	virtual void SetScaling(float scaleX, float scaleY, float scaleZ);
	virtual void SetScalingX(float scaleX);
	virtual void SetScalingY(float scaleY);
	virtual void SetScalingZ(float scaleZ);
	virtual const Vector3f& GetPosition() const { return mPosition; };
	virtual const Vector3f& GetScaling() const { return mScaling; };

	/**
	 @brief 获取物体局部变换矩阵
	 */
	virtual const Matrix44f& GetLocalTransform() const { return mTransform; };

	/**
	 @brief 更新函数
	 @param stepTime 每一帧增加的时间
	 */
	virtual void Update(float stepTime){};

	// ***************** event *******************
	virtual void OnPreUpdate(PreUpdateHandler handler) { mPreUpdateHandler = handler; };
	virtual void OnPostUpdate(PostUpdateHandler handler) { mPostUpdateHandler = handler; };

protected:
	bool mIsEnable;

	Vector3f mPosition;
	Vector3f mRotation;
	Vector3f mScaling;
	Matrix44f mTransform;

	PreUpdateHandler mPreUpdateHandler;
	PostUpdateHandler mPostUpdateHandler;
};

PE_END

#endif