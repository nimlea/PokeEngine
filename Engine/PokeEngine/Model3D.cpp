#include "Model3D.h"
#include <math.h>

#include <stdlib.h>
#include <algorithm>
#include <ctype.h>

#include "LoaderAssimp.h"

#pragma warning(disable:4018)

PE_BEGIN

Model3D::Model3D()
:mCurrentTime(0), mLastTime(0), mModelData(nullptr), mTexturePath(""), mIsStatic(true),
mIsPlay(false), mStart(0), mEnd(0)
{}

Model3D::~Model3D()
{
	for (unsigned int i = 0; i < mVertexGlobalTransform.size(); i++)
	{
		if (mVertexGlobalTransform[i] != nullptr) delete[] mVertexGlobalTransform[i];
	}
	mVertexGlobalTransform.clear();
}

std::shared_ptr<Model3D> Model3D::Create(const std::string &model, const std::string &texture /* = "" */)
{
	std::shared_ptr<Model3D> m3dObj(new Model3D());

	// ��ȡģ���ļ���һЩ��Ϣ
	char drive[8];
	char dir[256];
	char file_name[256];
	char ext[8];
	_splitpath_s(model.c_str(), drive, dir, file_name, ext);
	_strlwr_s(ext);

	m3dObj->mModelData = LoaderAssimp::Load(model);
	m3dObj->mTexturePath = texture;
	// ��ʼ������transform
	m3dObj->mVertexGlobalTransform.resize(m3dObj->mModelData->renderData->meshes.size());
	for (unsigned int i = 0; i < m3dObj->mVertexGlobalTransform.size(); i++)
	{
		std::shared_ptr<Meshdata> mesh = m3dObj->mModelData->renderData->meshes[i];
		m3dObj->mVertexGlobalTransform[i] = new Matrix44f[mesh->vertexCount];
	}
	// �Ƿ��Ǿ�̬ģ��
	if (m3dObj->mModelData->animations.size() > 0)
	{
		m3dObj->mIsStatic = false;
	}
	else
	{
		m3dObj->mIsStatic = true;
	}
	m3dObj->InitStaticOnce();

	return m3dObj;
}

void Model3D::Update(float stepTime)
{
	mCurrentTime += stepTime;

	// preUpdate event
	if (nullptr != mPreUpdateHandler)
	{
		mPreUpdateHandler(this);
	}

	// �ж���
	if (!mIsStatic && mIsPlay)
	{
		// ���ö���transform����
		for (unsigned int i = 0; i < mVertexGlobalTransform.size(); i++)
		{
			std::shared_ptr<Meshdata> mesh = mModelData->renderData->meshes[i];
			memset(mVertexGlobalTransform[i], 0, sizeof(Matrix44f)* mesh->vertexCount);
		}
		// ����
		RecursiveNode(mModelData->rootNode, 0);
		// �������ն���λ��
		for (unsigned int i = 0; i < mVertexGlobalTransform.size(); i++)
		{
			std::shared_ptr<Meshdata> mesh = mModelData->renderData->meshes[i];
			for (unsigned int j = 0; j < mesh->vertexCount; j++)
			{
				mesh->vertexData[j].pos = mModelData->meshes[i]->vertexData[j].pos * mVertexGlobalTransform[i][j].Transpose();
				mesh->vertexData[j].normal = mModelData->meshes[i]->vertexData[j].normal * mVertexGlobalTransform[i][j].Transpose();
			}
			/*for (unsigned int j = 0; j < mesh->vertexCount; j++)
			{
				mesh->vertexData[j].pos = mModelData->meshes[i]->vertexData[j].pos * mVertexGlobalTransform[i][j].Transpose() * mTransform;
				mesh->vertexData[j].normal = mModelData->meshes[i]->vertexData[j].normal * mVertexGlobalTransform[i][j].Transpose() * mTransform;
			}*/
		}
	}
	// ��̬ģ��
	else
	{
		for (unsigned int i = 0; i < mModelData->renderData->meshes.size(); i++)
		{
			std::shared_ptr<Meshdata> mesh = mModelData->renderData->meshes[i];
			// �Ѿ��ŵ�GPU������
			/*for (unsigned int j = 0; j < mesh->vertexCount; j++)
			{
			mesh->vertexData[j].pos = mModelData->meshes[i]->vertexData[j].pos * mTransform;
			mesh->vertexData[j].normal = mModelData->meshes[i]->vertexData[j].normal * mTransform;
			}*/
		}
	}

	// postUpdate event
	if (nullptr != mPostUpdateHandler)
	{
		mPostUpdateHandler(this);
	}

	mLastTime = mCurrentTime;
}

void Model3D::RecursiveNode(ModelNode *node, int animIndex /* = -1 */)
{
	// ���㶯��
	if (animIndex >= 0)
	{
		// �ж��Ƿ��ж���
		if (node->channelsID.size() > animIndex &&
			mModelData->animations.size() > animIndex)
		{
			// ����ڵ�ֲ��任
			int channelIndex = node->channelsID[animIndex];
			std::shared_ptr<AnimationData> anim = mModelData->animations[animIndex];
			if (channelIndex >= 0)
			{
				node->localTransform = CalculateAnimate(node, channelIndex, anim);
			}
		}
	}

	// ����node��global transform
	if (node->parent != nullptr)
	{
		node->globalTransform = node->parent->globalTransform * node->localTransform;
	}
	else
	{
		node->globalTransform = node->localTransform;
	}

	// �����������
	if ((node->type == E_ModelNodeType::eMNT_BONE) && 
		(node->boneData != nullptr))
	{
		Matrix44f &boneTransform = node->boneData->globalTransform;
		boneTransform = node->globalTransform * node->boneData->offsetTransform;

		// ���㶯���󶥵�λ��
		for (unsigned int i = 0; i < node->boneData->weight.size(); i++)
		{
			VertexWeight &weight = node->boneData->weight[i];
			mVertexGlobalTransform[node->boneData->meshID][weight.vertexID] += boneTransform * weight.value;
		}
	}

	// �ݹ�
	for (int i = 0; i < node->children.size(); i++)
	{
		RecursiveNode(node->children[i], animIndex);
	}
}

Matrix44f Model3D::CalculateAnimate(ModelNode *node, int channelIndex, std::shared_ptr<AnimationData> anim)
{
	AnimChannel &chl = anim->channels[channelIndex];
	// ��ʱ��
	//float t = mCurrentTime * anim->ticksPerSecond;
	//if (anim->duration > 0.0)
	//{
	//	t = fmod(t, anim->duration);
	//}
	// ��ȷ����Խ��
	float start_tick = std::max(0.0f, std::min(mStart, (float)anim->duration));
	float end_tick = std::max(start_tick, std::min(mEnd, (float)anim->duration));
	float currentTick = 0;
	float lastTick = 0;
	// ��������£����㱾֡�Ķ���tick����֡��tick
	if ((end_tick - start_tick) > 0)
	{
		currentTick = start_tick + fmod(mCurrentTime * anim->ticksPerSecond, end_tick - start_tick);
		lastTick = start_tick + fmod(mLastTime * anim->ticksPerSecond, end_tick - start_tick);
	}
	// ��ͣ�����ֻ���㱾֡��tick
	else
	{
		currentTick = start_tick;
	}

	// position
	Vector3f currentPosition(0, 0, 0);
	if (chl.positionKeys.size() > 0)
	{
		// �ҳ�ʱ�����ϵ�ǰtick��ǰһ֡�����뵱ǰtick�غϵ�֡��
		unsigned int frame = (currentTick >= lastTick ? node->lastPositionFrame : 0);
		while (frame < (chl.positionKeys.size() - 1))
		{
			if (currentTick < chl.positionKeys[frame + 1].time)
				break;
			frame++;
		}
		// ��ֵ
		unsigned int nextFrame = (frame + 1) % chl.positionKeys.size();
		AnimVectorKey key = chl.positionKeys[frame];
		AnimVectorKey nextKey = chl.positionKeys[nextFrame];
		double diffTime = nextKey.time - key.time;
		if (diffTime < 0.0){
			diffTime += anim->duration;
		}
		if (diffTime > 0.0)
		{
			float factor = (currentTick - key.time) / diffTime;
			currentPosition = key.value + (nextKey.value - key.value) * factor;
		}
		else
		{
			currentPosition = key.value;
		}
		// ���浱ǰ֡
		node->lastPositionFrame = frame;
	}
	// rotation
	aiQuaternion currentRotation(1, 0, 0, 0);
	if (chl.rotationKeys.size() > 0)
	{
		unsigned int frame = (currentTick >= lastTick ? node->lastRotationFrame : 0);
		while (frame < (chl.rotationKeys.size() - 1))
		{
			if (currentTick < chl.rotationKeys[frame + 1].time)
				break;
			frame++;
		}
		unsigned int nextFrame = (frame + 1) % chl.rotationKeys.size();
		AnimQuatKey key = chl.rotationKeys[frame];
		AnimQuatKey nextKey = chl.rotationKeys[nextFrame];
		double diffTime = nextKey.time - key.time;
		if (diffTime < 0){
			diffTime += anim->duration;
		}
		if (diffTime > 0)
		{
			float factor = (currentTick - key.time) / diffTime;
			aiQuaternion::Interpolate(currentRotation, key.value, nextKey.value, factor);
		}
		else
		{
			currentRotation = key.value;
		}
		node->lastRotationFrame = frame;
	}
	// scaling
	Vector3f currentScaling(1, 1, 1);
	if (chl.scalingKeys.size() > 0)
	{
		unsigned int frame = (currentTick >= lastTick) ? node->lastScalingFrame : 0;
		while (frame < (chl.scalingKeys.size() - 1))
		{
			if (currentTick < chl.scalingKeys[frame + 1].time)
				break;
			frame++;
		}
		// ����ֵ����ɢ��
		currentScaling = chl.scalingKeys[frame].value;
		node->lastScalingFrame = frame;
	}
	// return transform
	aiMatrix4x4 rm = aiMatrix4x4(currentRotation.GetMatrix());
	Matrix44f mat = Matrix44f(
		rm.a1, rm.a2, rm.a3, rm.a4,
		rm.b1, rm.b2, rm.b3, rm.b4,
		rm.c1, rm.c2, rm.c3, rm.c4,
		rm.d1, rm.d2, rm.d3, rm.d4);
	mat.m11 *= currentScaling.x; mat.m12 *= currentScaling.y; mat.m13 *= currentScaling.z; mat.m14 = currentPosition.x;
	mat.m21 *= currentScaling.x; mat.m22 *= currentScaling.y; mat.m23 *= currentScaling.z; mat.m24 = currentPosition.y;
	mat.m31 *= currentScaling.x; mat.m32 *= currentScaling.y; mat.m33 *= currentScaling.z; mat.m34 = currentPosition.z;
	return mat;
}

void Model3D::InitStaticOnce()
{
	RecursiveNode(mModelData->rootNode);
}

void Model3D::Play(float start, float end /* = -1 */)
{
	if (!mIsPlay)
	{
		mStart = start;
		mEnd = end;
		mCurrentTime = 0;
		mIsPlay = true;
	}
}

void Model3D::Stop()
{
	mIsPlay = false;
}

PE_END