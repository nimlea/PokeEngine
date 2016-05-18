#include "SceneObject.h"
#include <math.h>
#include "..\..\Libs\libMath\MathUtility.h"

PE_BEGIN

SceneObject::SceneObject()
{
	mIsEnable = true;

	mPosition.Zero();
	mRotation.Zero();
	mScaling.Set(1.0f, 1.0f, 1.0f);

	mPreUpdateHandler = nullptr;
	mPostUpdateHandler = nullptr;

	mTransform.Indentity();
}

SceneObject::~SceneObject()
{}

void SceneObject::SetPosition(const Vector3f &p)
{
	SetPosition(p.x, p.y, p.z);
}

void SceneObject::SetPosition(float x, float y, float z)
{
	SetPositionX(x);
	SetPositionY(y);
	SetPositionZ(z);
}

void SceneObject::SetPositionX(float x)
{
	mTransform.m41 = x;
	mPosition.x = x;
}

void SceneObject::SetPositionY(float y) 
{
	mTransform.m42 = y;
	mPosition.y = y;
}

void SceneObject::SetPositionZ(float z) 
{
	mTransform.m43 = z;
	mPosition.z = z;
}

/**
 |1  0    0    0|
 |0  cos¦È sin¦È 0|
 |0 -sin¦È cos¦È 0|
 |0  0    0    1|
 */
void SceneObject::SetRotationXByDegrees(float angel)
{
	float degrees = fmod((angel - mRotation.x), 360);
	float rad = degrees / 180 * MATH_PI;
	Matrix44f rotM(
		1, 0, 0, 0,
		0, cos(rad), sin(rad), 0,
		0, -sin(rad), cos(rad), 0,
		0, 0, 0, 1
		);
	mTransform = rotM * mTransform;
	mRotation.x = angel;
}

/**
|cos¦È 0 -sin¦È 0|
|0    1  0    0|
|sin¦È 0  cos¦È 0|
|0    0  0    1|
*/
void SceneObject::SetRotationYByDegrees(float angel)
{
	float degrees = fmod((angel - mRotation.y), 360);
	float rad = degrees / 180 * MATH_PI;
	Matrix44f rotM(
		cos(rad), 0, -sin(rad), 0,
		0, 1, 0, 0,
		sin(rad), 0, cos(rad), 0,
		0, 0, 0, 1
		);
	mTransform = rotM * mTransform;
	mRotation.y = angel;
}

/**
| cos¦È sin¦È 0 0|
|-sin¦È cos¦È 0 0|
| 0    0    1 0|
| 0    0    0 1|
*/
void SceneObject::SetRotationZByDegrees(float angel)
{
	float degrees = fmod((angel - mRotation.z), 360);
	float rad = degrees / 180 * MATH_PI;
	Matrix44f rotM(
		cos(rad), sin(rad), 0, 0,
		-sin(rad), cos(rad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
	mTransform = rotM * mTransform;
	mRotation.z = angel;
}

void SceneObject::SetScaling(float scale)
{
	SetScaling(scale, scale, scale);
}

void SceneObject::SetScaling(float scaleX, float scaleY, float scaleZ)
{
	SetScalingX(scaleX);
	SetScalingY(scaleY);
	SetScalingZ(scaleZ);
}

void SceneObject::SetScalingX(float scaleX)
{
	mTransform.m11 *= (scaleX / mScaling.x);
	mScaling.x = scaleX;
}

void SceneObject::SetScalingY(float scaleY)
{
	mTransform.m22 *= (scaleY / mScaling.y);
	mScaling.y = scaleY;
}

void SceneObject::SetScalingZ(float scaleZ)
{
	mTransform.m33 *= (scaleZ / mScaling.z);
	mScaling.z = scaleZ;
}

PE_END