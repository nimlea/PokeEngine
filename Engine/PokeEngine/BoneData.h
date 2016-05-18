#pragma once
#ifndef _BONEDATA_H_
#define _BONEDATA_H_

#include "..\Common\PEMacro.h"
#include <vector>
#include "..\..\Libs\libMath\Matrix44.h"

PE_BEGIN

// ����Ӱ��Ķ���Ȩ��
struct VertexWeight
{
	unsigned int vertexID = 0;
	float value = 0;
};

// ������������
struct BoneData
{
	int meshID = -1; // �������ڵ�Mesh
	std::vector<VertexWeight> weight; // Ȩ������
	Matrix44f globalTransform;
	Matrix44f offsetTransform; // ��Mesh�������󶨿ռ�ľ��󣬼��㶯������ʹ��
};

PE_END

#endif