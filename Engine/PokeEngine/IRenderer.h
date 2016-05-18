#pragma once
#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include "SceneManager.h"

PE_BEGIN

class IRenderer
{
public:
	virtual ~IRenderer(){};

	/**
	 @brief ��ʼ����Ⱦ��
	 @return �ɹ�����1��ʧ�ܷ���0
	 */
	virtual int Init(void *hWnd) = 0;
	virtual void Release() = 0;
	virtual void Render(const std::shared_ptr<SceneManager> sceneMgr) = 0;
};

PE_END

#endif