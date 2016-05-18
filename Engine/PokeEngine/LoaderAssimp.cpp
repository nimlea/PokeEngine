#include "LoaderAssimp.h"

#include "..\..\Libs\assimp\include\cimport.h"
#include "..\..\Libs\assimp\include\scene.h"
#include "..\..\Libs\assimp\include\postprocess.h"
#include <algorithm>
#include "LogSystem.h"

#pragma warning(disable:4018)

PE_BEGIN

std::shared_ptr<ModelData> LoaderAssimp::Load(const std::string path)
{
	LogSystem::GetInstance().Trace("%s开始载入资源文件：%s%s", "\n", path.c_str(), "\n");
	std::shared_ptr<ModelData> data_out;
	// 读取assimp格式
	Progress(0, "读取文件内容");
	unsigned int flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_FlipUVs;
	aiPropertyStore* props = aiCreatePropertyStore();
	aiSetImportPropertyInteger(props, "import_props", aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	const aiScene *scene = aiImportFileExWithProperties(path.c_str(), flags, nullptr, props);
	aiReleasePropertyStore(props);
	// 处理数据
	Progress(30, "转换文件数据");
	data_out = LoaderAssimp::ProcessSceneData(scene);
	// 释放资源
	aiReleaseImport(scene);
	Progress(100, "载入完成\n");

	return data_out;
}

std::shared_ptr<ModelData> LoaderAssimp::ProcessSceneData(const aiScene *scene)
{
	std::shared_ptr<ModelData> model(new ModelData());

	// 初始化渲染数据
	model->renderData.reset(new RenderData());

	// 获取Node树结构
	Progress(35, "获取节点数据");
	model->rootNode = RecursiveNodeData(scene->mRootNode);

	// 获取Mesh数据
	Progress(50, "获取网格数据");
	model->meshes.clear();
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		const aiMesh *assimpMesh = scene->mMeshes[i];
		std::shared_ptr<Meshdata> mesh = ProcessMeshData(assimpMesh);
		// 递归转换骨骼数据到Node
		if (assimpMesh->HasBones())
		{
			model->rootNode->boneData = RecursiveBoneData(model->rootNode, assimpMesh, i);
			if (model->rootNode->boneData != nullptr)
			{
				model->rootNode->type = E_ModelNodeType::eMNT_BONE;
			}
		}

		model->meshes.push_back(mesh);
	}

	// 获取动画数据
	Progress(70, "获取动画数据");
	model->animations.clear();
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		std::shared_ptr<AnimationData> anim = ProcessAnimationData(scene->mAnimations[i]);
		RecursiveAnimData(model->rootNode, i, anim);
		model->animations.push_back(anim);
	}

	// 初始化渲染用mesh
	Progress(85, "生成渲染数据");
	RecursiveInitRenderMesh(model->rootNode, model);

	return model;
}

void LoaderAssimp::RecursiveAnimData(ModelNode *node, int animIndex, std::shared_ptr<AnimationData> animData)
{
	for (unsigned int i = 0; i < animData->channels.size(); i++)
	{
		if (0 == strcmp(node->name, animData->channels[i].name))
		{
			if (node->channelsID.size() <= animIndex)
			{
				node->channelsID.resize(animIndex + 1, -1);
			}
			node->channelsID[animIndex] = i;
		}
	}
	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		RecursiveAnimData(node->children[i], animIndex, animData);
	}
}

void LoaderAssimp::RecursiveInitRenderMesh(ModelNode *node, std::shared_ptr<ModelData> model)
{
	if (node->type == E_ModelNodeType::eMNT_MESH)
	{
		for (int i = 0; i < node->modelMeshesID.size(); i++)
		{
			// 初始化
			std::shared_ptr<Meshdata> mesh = model->meshes[node->modelMeshesID[i]];
			std::shared_ptr<Meshdata> data(new Meshdata());
			data->vertexCount = mesh->vertexCount;
			data->vertexData = new VertexData[data->vertexCount];
			memcpy(data->vertexData, mesh->vertexData, sizeof(VertexData)*data->vertexCount);
			data->indexCount = mesh->indexCount;
			data->indexData = new unsigned int[data->indexCount];
			memcpy(data->indexData, mesh->indexData, sizeof(unsigned int)*data->indexCount);
			// 设置
			model->renderData->meshes.push_back(data);
			node->renderMeshesID.push_back((model->renderData->meshes.size() - 1));
		}
	}
	// 子节点
	for (int i = 0; i < node->children.size(); i++)
	{
		RecursiveInitRenderMesh(node->children[i], model);
	}
}

std::shared_ptr<AnimationData> LoaderAssimp::ProcessAnimationData(const aiAnimation *assimpAnim)
{
	std::shared_ptr<AnimationData> anim(new AnimationData());
	// animation name
	memcpy(anim->name, assimpAnim->mName.data, strlen(assimpAnim->mName.data) + 1);
	// duration
	anim->duration = assimpAnim->mDuration;
	// ticks / second
	anim->ticksPerSecond = (assimpAnim->mTicksPerSecond != 0 ? assimpAnim->mTicksPerSecond : 25.0);
	// channel and key frame
	anim->channels.clear();
	for (int i = 0; i < assimpAnim->mNumChannels; i++)
	{
		aiNodeAnim *assimpNodeAnim = assimpAnim->mChannels[i];
		AnimChannel channel;
		// channel name
		memcpy(channel.name, assimpNodeAnim->mNodeName.data, strlen(assimpNodeAnim->mNodeName.data) + 1);
		// position key frame
		channel.positionKeys.clear();
		for (unsigned int positionIndex = 0; positionIndex < assimpNodeAnim->mNumPositionKeys; positionIndex++)
		{
			AnimVectorKey position;
			position.time = assimpNodeAnim->mPositionKeys[positionIndex].mTime;
			position.value = LoaderAssimp::GetVector3(assimpNodeAnim->mPositionKeys[positionIndex].mValue);
			channel.positionKeys.push_back(position);
		}
		// rotation key frame
		channel.rotationKeys.clear();
		for (unsigned int rotationIndex = 0; rotationIndex < assimpNodeAnim->mNumRotationKeys; rotationIndex++)
		{
			AnimQuatKey rotation;
			rotation.time = assimpNodeAnim->mRotationKeys[rotationIndex].mTime;
			rotation.value = assimpNodeAnim->mRotationKeys[rotationIndex].mValue;
			channel.rotationKeys.push_back(rotation);
		}
		// scale key frame
		channel.scalingKeys.clear();
		for (unsigned int scalingIndex = 0; scalingIndex < assimpNodeAnim->mNumScalingKeys; scalingIndex++)
		{
			AnimVectorKey scaling;
			scaling.time = assimpNodeAnim->mScalingKeys[scalingIndex].mTime;
			scaling.value = LoaderAssimp::GetVector3(assimpNodeAnim->mScalingKeys[scalingIndex].mValue);
			channel.scalingKeys.push_back(scaling);
		}
		anim->channels.push_back(channel);
	}

	return anim;
}

Vector3f LoaderAssimp::GetVector3(const aiVector3D &v)
{
	Vector3f vec;
	vec.x = v.x;
	vec.y = v.y;
	vec.z = v.z;
	return vec;
}

BoneData* LoaderAssimp::RecursiveBoneData(const ModelNode *node, const aiMesh *assimpMesh, int meshIndex)
{
	BoneData *bone = nullptr;
	for (int i = 0; i < assimpMesh->mNumBones; i++)
	{
		// 如果为骨骼节点，转换骨骼数据
		if (0 == strcmp(node->name, assimpMesh->mBones[i]->mName.data))
		{
			aiBone *assimpBone = assimpMesh->mBones[i];
			bone = new BoneData();
			bone->meshID = meshIndex;
			bone->offsetTransform = LoaderAssimp::GetMatrix44(assimpBone->mOffsetMatrix);
			bone->weight.clear();
			for (int j = 0; j < assimpBone->mNumWeights; j++)
			{
				VertexWeight w;
				w.vertexID = assimpBone->mWeights[j].mVertexId;
				w.value = assimpBone->mWeights[j].mWeight;
				bone->weight.push_back(w);
			}
			break;
		}
	}
	// 递归
	for (int i = 0; i < node->children.size(); i++)
	{
		ModelNode *childNode = node->children[i];
		childNode->boneData = RecursiveBoneData(childNode, assimpMesh, meshIndex);
		if (childNode->boneData != nullptr)
		{
			childNode->type = E_ModelNodeType::eMNT_BONE;
		}
	}

	return bone;
}

ModelNode* LoaderAssimp::RecursiveNodeData(const aiNode *assimpNode)
{
	ModelNode* modeNode = new ModelNode();
	// name
	memcpy(modeNode->name, assimpNode->mName.data, strlen(assimpNode->mName.data) + 1);
	// transform
	modeNode->localTransform = LoaderAssimp::GetMatrix44(assimpNode->mTransformation);
	modeNode->globalTransform.Indentity();
	// meshes
	if (assimpNode->mNumMeshes > 0)
	{
		modeNode->type = E_ModelNodeType::eMNT_MESH;
	}
	modeNode->modelMeshesID.clear();
	for (int i = 0; i < assimpNode->mNumMeshes; i++)
	{
		modeNode->modelMeshesID.push_back(assimpNode->mMeshes[i]);
	}
	// children
	modeNode->children.clear();
	for (int i = 0; i < assimpNode->mNumChildren; i++)
	{
		ModelNode *childNode = LoaderAssimp::RecursiveNodeData(assimpNode->mChildren[i]);
		childNode->parent = modeNode;
		modeNode->children.push_back(childNode);
	}
	return modeNode;
}

Matrix44f LoaderAssimp::GetMatrix44(const aiMatrix4x4 &m)
{
	Matrix44f mat(
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
	return mat;
}

std::shared_ptr<Meshdata> LoaderAssimp::ProcessMeshData(const aiMesh *assimpMesh)
{
	std::shared_ptr<Meshdata> mesh(new Meshdata());
	// loop vertices
	mesh->vertexCount = assimpMesh->mNumVertices;
	mesh->vertexData = new VertexData[mesh->vertexCount];
	for (int i = 0; i < assimpMesh->mNumVertices; i++)
	{
		VertexData &vertex = mesh->vertexData[i];
		// position
		const aiVector3D &posValue = assimpMesh->mVertices[i];
		vertex.pos.x = posValue.x;
		vertex.pos.y = posValue.y;
		vertex.pos.z = posValue.z;
		// color
		if (assimpMesh->HasVertexColors(0))
		{
			const aiColor4D &colorValue = assimpMesh->mColors[0][i];
			vertex.color.x = colorValue.r;
			vertex.color.y = colorValue.g;
			vertex.color.z = colorValue.b;
			vertex.color.w = colorValue.a;
		}
		else
		{
			vertex.color.x = vertex.color.y = vertex.color.z = vertex.color.w = 1.0f;
		}
		// uv
		Vector2f uvValue;
		for (int uvIndex = 0; uvIndex < MAX_UV_COUNT; uvIndex++)
		{
			if (assimpMesh->HasTextureCoords(uvIndex))
			{
				vertex.uv[uvIndex].x = assimpMesh->mTextureCoords[uvIndex][i].x;
				vertex.uv[uvIndex].y = assimpMesh->mTextureCoords[uvIndex][i].y;
			}
			else
			{
				vertex.uv[uvIndex].x = 0.0f;
				vertex.uv[uvIndex].y = 0.0f;
			}
		}
		// normal
		if (assimpMesh->HasNormals())
		{
			vertex.normal.x = assimpMesh->mNormals[i].x;
			vertex.normal.y = assimpMesh->mNormals[i].y;
			vertex.normal.z = assimpMesh->mNormals[i].z;
		}
	}
	// loop indices
	mesh->indexCount = assimpMesh->mNumFaces * assimpMesh->mFaces[0].mNumIndices;
	mesh->indexData = new unsigned int[mesh->indexCount];
	int indexCount = 0;
	for (unsigned int faceIndex = 0; faceIndex < assimpMesh->mNumFaces; faceIndex++)
	{
		aiFace &face = assimpMesh->mFaces[faceIndex];
		for (unsigned int indexInFace = 0; indexInFace < face.mNumIndices; indexInFace++)
		{
			mesh->indexData[indexCount] = face.mIndices[indexInFace];
			indexCount++;
		}
	}
	// loop bone
	//int *vertexBoneCount = new int[mesh->vertexCount]; // 每个顶点的骨骼计数
	//memset(vertexBoneCount, 0, sizeof(unsigned int)* mesh->vertexCount); // 初始化为0
	//for (unsigned int boneIndex = 0; boneIndex < assimpMesh->mNumBones; boneIndex++)
	//{
	//	aiBone *bone = assimpMesh->mBones[boneIndex];
	//	for (unsigned int weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++)
	//	{
	//		aiVertexWeight &weight = bone->mWeights[weightIndex];
	//		unsigned int count = std::min(vertexBoneCount[weight.mVertexId], 3); // 顶点的最大影响骨骼数不超过4个
	//		// 赋值骨骼编号和权重
	//		mesh->vertexData[weight.mVertexId].bone[count] = boneIndex;
	//		mesh->vertexData[weight.mVertexId].weight[count] = weight.mWeight;
	//		// 计数器+1
	//		vertexBoneCount[weight.mVertexId] = count + 1;
	//	}
	//}
	//delete[] vertexBoneCount;

	return mesh;
}

void LoaderAssimp::Progress(float percent, const char *action)
{
	LogSystem::GetInstance().Trace("%s                                                                   ", "\r"); // 先清空本行百分比数据
	LogSystem::GetInstance().Trace("%s载入中 %.2f%s，%s", "\r", percent, "%%", action);
}

PE_END