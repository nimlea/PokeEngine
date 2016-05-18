#pragma once
#ifndef _ANIMATIONDATA_H_
#define _ANIMATIONDATA_H_

#include <vector>
#include "..\Common\PEMacro.h"
#include "..\..\Libs\assimp\include\scene.h"
#include "..\..\Libs\assimp\include\quaternion.h"

PE_BEGIN

/*
* vector���͵Ĺؼ�֡�ṹ
*/
struct AnimVectorKey
{
	double time;
	Vector3f value;
};

/*
* ��Ԫ�����͵Ĺؼ�֡�ṹ
*/
struct AnimQuatKey
{
	double time;
	aiQuaternion value;
};

/*
* ������Channel��һ��Channel��������һ��Node�ڵ㣬Channel��name�ͽڵ��name��ͬ
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
	double duration; // ���ڣ��ܵ�ticks
	double ticksPerSecond; // ��������
	std::vector<AnimChannel> channels;

	AnimationData()
	{
		memset(name, '\0', sizeof(name));
	}
};

PE_END

#endif