#include "PokeApp.h"

#include "LogSystem.h"
#include "RendererD3D11.h"
#include "..\Common\PEMacro.h"

#pragma warning(disable:4244)

PE_BEGIN

std::shared_ptr<PokeApp> PokeApp::mAppInst = nullptr; // 初始化一下类里的静态实例

std::shared_ptr<PokeApp> PokeApp::Create(const char *title, int width, int height, E_RenderMode renderMode /* = E_RenderMode::eRM_DX_11 */)
{
	// 只保存一个app实例
	if (PokeApp::mAppInst != nullptr){
		LogSystem::GetInstance().Log("创建失败，已有一个或多个application");
		return mAppInst;
	}

	// 桌面分辨率和程序大小
	PokeApp *_app = new PokeApp();
	std::shared_ptr<PokeApp> app(_app);
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	app->SetResulotionW(sw);
	app->SetResulotionH(sh);
	app->SetScreenW(min(sw, width));
	app->SetScreenH(min(sh, height));

	// 注册窗口类
	WNDCLASSEX wcex;
	HINSTANCE hInst = GetModuleHandle(nullptr);
	char *strClassName = "app_class_name";
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = PokeApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = strClassName;
	wcex.hIconSm = nullptr;
	if (!RegisterClassEx(&wcex)){
		LogSystem::GetInstance().Log("注册窗口类失败");
		return nullptr;
	}

	// 创建窗口
	float center_x = (app->GetResulotionW() - app->GetScreenW()) / 2;
	float center_y = (app->GetResulotionH() - app->GetScreenH()) / 2;
	HWND hWnd = CreateWindowEx(WS_EX_LEFT, strClassName, title, WS_OVERLAPPEDWINDOW, 
								center_x, center_y, app->GetScreenW(), app->GetScreenH(), nullptr, nullptr, hInst, nullptr);
	if (hWnd == nullptr)
	{
		LogSystem::GetInstance().Log("创建窗口失败，hWnd为NULL");
		return nullptr;
	}
	else
	{
		app->SetHWnd(hWnd);
	}

	// 创建渲染器
	app->SetRenderMode(renderMode);

	// 初始化
	if (FAILED(app->Init())){
		return nullptr;
	}
	LogSystem::GetInstance().Log("创建application成功");
	PokeApp::mAppInst = app;
	return app;
}

LRESULT CALLBACK PokeApp::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch(nMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}
	//调用缺省的消息处理程序  
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

PokeApp::PokeApp()
{
	mResulotionW = 800;
	mResulotionH = 600;
	mScreenW = 640;
	mScreenH = 480;
	SetFPS(60);

	mHWnd = nullptr;

	mRenderer = nullptr;
}

PokeApp::~PokeApp()
{
#ifdef PE_DEBUG_MODE
	LogSystem::GetInstance().Log("析构PEApplicaition");
#endif
	Release();
}

int PokeApp::Init()
{
	RECT rc;
	GetClientRect((HWND)mHWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	// scene manager
	mSceneMgr.reset(new SceneManager());
	mSceneMgr->Init((float)width / height);

	// renderer
	if (mRenderer != nullptr)
	{
		if (0 == mRenderer->Init(mHWnd))
		{
			return 0;
		}
	}

	// input system
	mInputSys.reset(new InputSystem());
	mInputSys->Init(mHWnd);

	// audio system
	mAudioSys.reset(new AudioSystem());
	mAudioSys->Init();

	return 1;
}

void PokeApp::Release()
{
	PostMessage(mHWnd, WM_CLOSE, 0, 0);
}

void PokeApp::SetRenderMode(E_RenderMode mode)
{
	IRenderer *_renderer = nullptr;
	switch (mode)
	{
	case E_RenderMode::eRM_DX_11:
		_renderer = new RendererD3D11();
		mRenderer.reset(_renderer);
		break;
	default:
		break;
	}
}

int PokeApp::Run()
{
	ShowWindow(mHWnd, SW_SHOW);
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 更新一帧
		if (mTimer.GetSeconds() >= mSecondsPerFrame)
		{
			float dt = mTimer.GetSeconds();
			// reset timer
			mTimer.Reset();

			mInputSys->UpdateInputState();
			mSceneMgr->Update(dt);
			mRenderer->Render(mSceneMgr);
		}
	}

	return (int)msg.wParam;
}

void PokeApp::SetFPS(int fps)
{
	mFramsPerSecond = fps;
	mSecondsPerFrame = 1.0f / mFramsPerSecond;
}

PE_END