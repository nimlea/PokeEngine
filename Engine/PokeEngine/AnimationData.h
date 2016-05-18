#pragma once
#ifndef _ANIMATIONDATA_H_
#define _ANIMATIONDATA_H_

#include <vector>
#include "..\Common\PEMacro.h"
#include "..\..\Libs\assimp\include\scene.h"
#include "..\..\Libs\assimp\include\quaternion.h"

PE_BEGIN

/*
* vector类型的关键帧结构
*/
struct AnimVectorKey
{
	double time;
	Vector3f value;
};

/*
* 四元数类型的关键帧结构
*/
struct AnimQuatKey
{
	double time;
	aiQuaternion value;
};

/*
* 动画的Channel，一个Channel用来控制一个Node节点，Channel的name和节点的name相同
*/
struct AnimChannel
{
	char name[256];

	std::vector<AnimVectorKey> positionKeys;
	std::vector<AnimQuatKey> rotationKeys;
	std::vector<AnimVectorKey> scalingKeys;

	AnimChannel()
	{
		memset(name, '\0', sizeof(name));
	}
};

struct AnimationData
{
	char name[256];
	double duration; // 周期，总的ticks
	double ticksPerSecond; // 动画速率
	std::vector<AnimChannel> channels;

	AnimationData()
	{
		memset(name, '\0', sizeof(name));
	}
};

PE_END

#endif