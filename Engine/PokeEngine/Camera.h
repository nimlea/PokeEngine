#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "SceneObject.h"

PE_BEGIN

class Camera :public SceneObject
{
public:
	Camera();

	void SetLookat(float x, float y, float z);
	void SetLookat(const Vector3f &p);

	Vector3f& GetUp() { return mUp; };
	Vector3f& GetLookat() { return mLookPoint; };

private:
	Vector3f mLookPoint;
	Vector3f mUp;
};

PE_END

#endif