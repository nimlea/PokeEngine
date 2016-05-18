#pragma once
#ifndef _BONEDATA_H_
#define _BONEDATA_H_

#include "..\Common\PEMacro.h"
#include <vector>
#include "..\..\Libs\libMath\Matrix44.h"

PE_BEGIN

// 骨骼影响的顶点权重
struct VertexWeight
{
	unsigned int vertexID = 0;
	float value = 0;
};

// 单个骨骼数据
struct BoneData
{
	int meshID = -1; // 骨骼所在的Mesh
	std::vector<VertexWeight> weight; // 权重数据
	Matrix44f globalTransform;
	Matrix44f offsetTransform; // 从Mesh到骨骼绑定空间的矩阵，计算动画数据使用
};

PE_END

#endif