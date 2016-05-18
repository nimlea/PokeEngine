#ifndef _PokeApp_H_
#define _PokeApp_H_

#include <Windows.h>
#include "EngineTimer.h"
#include "IRenderer.h"
#include "SceneManager.h"
#include "InputSystem.h"
#include "AudioSystem.h"

PE_BEGIN

enum E_RenderMode
{
	eRM_NONE = 0,
	eRM_DX_11 = 1,
};

class PokeApp
{
public:
	static std::shared_ptr<PokeApp> Create(const char *title, int width, int height, E_RenderMode renderMode = E_RenderMode::eRM_DX_11);
	static std::shared_ptr<PokeApp> GetInstance(){ return mAppInst; };

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	static std::shared_ptr<PokeApp> mAppInst;	// ���洴����ʵ����ֻ�ܴ���һ��app

public:
	~PokeApp();

	int Run();

	// ϵͳ�ֱ��ʴ�С
	void SetResulotionW(int w){ mResulotionW = w; };
	void SetResulotionH(int h){ mResulotionH = h; };
	int GetResulotionW() const { return mResulotionW; };
	int GetResulotionH() const { return mResulotionH; };

	// ���ڴ�С
	void SetScreenW(int w){ mScreenW = w; };
	void SetScreenH(int h){ mScreenH = h; };
	int GetScreenW() const { return mScreenW; };
	int GetScreenH() const { return mScreenH; };

	// ���ھ��
	void SetHWnd(HWND hWnd){ mHWnd = hWnd; };
	HWND GetHWnd() const { return mHWnd; };

	// ����������
	std::shared_ptr<SceneManager> GetSceneManager() const { return mSceneMgr; };

	// ��Ⱦ��
	std::shared_ptr<IRenderer> GetRenderer() const { return mRenderer; };

	// FPS
	void SetFPS(int fps);
	int GetFPS() const { return mFramsPerSecond; };

	std::shared_ptr<InputSystem> GetInputSystem() { return mInputSys; };

	std::shared_ptr<AudioSystem> GetAudioSystem() { return mAudioSys; };

private:
	PokeApp();

	int Init();
	void Release();

	/**
	 @brief ������Ⱦ��
	 @mode ��Ⱦģʽ
	 */
	void SetRenderMode(E_RenderMode mode);

private:
	int	mResulotionW, mResulotionH;
	int mScreenW, mScreenH;

	EngineTimer mTimer;
	// frames per second
	int mFramsPerSecond; // Ĭ��60
	float mSecondsPerFrame;

	HWND mHWnd;
	std::shared_ptr<SceneManager> mSceneMgr;
	std::shared_ptr<IRenderer> mRenderer;
	std::shared_ptr<InputSystem> mInputSys;
	std::shared_ptr<AudioSystem> mAudioSys;
};

PE_END

#endif