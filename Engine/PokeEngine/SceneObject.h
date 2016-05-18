#pragma once
#ifndef _SCENEOBJECT_H_
#define _SCENEOBJECT_H_

#include <memory>
#include "..\Common\PEMacro.h"
#include "..\..\Libs\libMath\Vector3.h"
#include "..\..\Libs\libMath\Matrix44.h"

PE_BEGIN

/**
@brief ����ǰ����
@param self SceneObject�������ָ��
*/
typedef void(*PreUpdateHandler)(void *self);
/**
@brief ����������
@param self SceneObject�������ָ��
*/
typedef void(*PostUpdateHandler)(void *self);

/**
 @brief ��Ϊ�����������Ԫ�ص��࣬�����е�Ԫ�ض��̳��Դ���
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
	 @brief ����λ�ã�xyzֵΪֱ��ֵ����λ������
	 */
	virtual void SetPosition(const Vector3f &p);
	/**
	@brief ����λ�ã�xyzֵΪֱ��ֵ����λ������
	*/
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPositionX(float x);
	virtual void SetPositionY(float y);
	virtual void SetPositionZ(float z);
	/**
	 @brief ��ģ������ĽǶ�Ϊ��׼��ת
	 @param angel ������ת�Ƕ�
	 */
	virtual void SetRotationXByDegrees(float angel);
	virtual void SetRotationYByDegrees(float angel);
	virtual void SetRotationZByDegrees(float angel);
	/**
	 @brief ��������
	 @param scale ���ű���
	 */
	virtual void SetScaling(float scale);
	/**
	 @brief �ֱ����Ÿ�����
	 */
	virtual void SetScaling(float scaleX, float scaleY, float scaleZ);
	virtual void SetScalingX(float scaleX);
	virtual void SetScalingY(float scaleY);
	virtual void SetScalingZ(float scaleZ);
	virtual const Vector3f& GetPosition() const { return mPosition; };
	virtual const Vector3f& GetScaling() const { return mScaling; };

	/**
	 @brief ��ȡ����ֲ��任����
	 */
	virtual const Matrix44f& GetLocalTransform() const { return mTransform; };

	/**
	 @brief ���º���
	 @param stepTime ÿһ֡���ӵ�ʱ��
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