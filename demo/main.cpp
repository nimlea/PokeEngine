#include "..\Engine\PokeEngine\PokeApp.h"
#include "..\Engine\PokeEngine\LogSystem.h"

using namespace std;
using namespace pe;

void CharactorMove(Model3D *model)
{
	auto keyboard = PokeApp::GetInstance()->GetInputSystem();
	auto position = model->GetPosition();
	float distance = 0.7f;
	if (keyboard->GetKeyDownState(InputKeys::Up))
	{
		model->Play(0, 24);
		position.z -= distance;
		model->SetRotationYByDegrees(180);
	}
	else if (keyboard->GetKeyDownState(InputKeys::Down))
	{
		model->Play(0, 24);
		position.z += distance;
		model->SetRotationYByDegrees(0);
	}
	else if (keyboard->GetKeyDownState(InputKeys::Left))
	{
		model->Play(0, 24);
		position.x -= distance;
		model->SetRotationYByDegrees(-90);
	}
	else if (keyboard->GetKeyDownState(InputKeys::Right))
	{
		model->Play(0, 24);
		position.x += distance;
		model->SetRotationYByDegrees(90);
	}
	else
	{
		model->Stop();
	}
	// 简单的碰撞
	position.z = max(position.z, -30);
	position.z = min(position.z, 30);
	position.x = max(position.x, -44);
	position.x = min(position.x, 43);
	// 楼梯
	if (position.x > 30 && position.z < -16)
	{
		if (keyboard->GetKeyDownState(InputKeys::Up)){
			position.z = max(position.z, -16);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Right)){
			position.x = min(position.x, 30);
		}
	}
	// 柜子
	if (position.x < 1 && position.z < -20)
	{
		if (keyboard->GetKeyDownState(InputKeys::Up)){
			position.z = max(position.z, -20);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Left)){
			position.x = max(position.x, 1);
		}
	}
	// 床
	if (position.x > -40 && position.x < -16 && position.z > -5 && position.z < 23)
	{
		if (keyboard->GetKeyDownState(InputKeys::Up))
		{
			position.z = max(position.z, 23);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Down))
		{
			position.z = min(position.z, -5);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Left))
		{
			position.x = max(position.x, -16);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Right))
		{
			position.x = min(position.x, -40);
		}
	}
	// 电视
	if (position.x > -11 && position.x < 11 && position.z > -10 && position.z < 20)
	{
		if (keyboard->GetKeyDownState(InputKeys::Up))
		{
			position.z = max(position.z, 20);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Down))
		{
			position.z = min(position.z, -10);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Left))
		{
			position.x = max(position.x, 11);
		}
		else if (keyboard->GetKeyDownState(InputKeys::Right))
		{
			position.x = min(position.x, -11);
		}
	}

	model->SetPosition(position);
}

void LoadScene()
{
	auto scene = PokeApp::GetInstance()->GetSceneManager();

	auto objRoom = Model3D::Create("resource\\room.dae");
	objRoom->SetRotationXByDegrees(-90);
	scene->AddModel3D(objRoom);
	auto m3dCharacter = Model3D::Create("resource\\charactor.fbx");
	m3dCharacter->SetPositionZ(20);
	m3dCharacter->OnPreUpdate(
		[](void *self)
	{
		Model3D *model = (Model3D*)self;
		CharactorMove(model);
	});
	scene->AddModel3D(m3dCharacter);
	auto m3dComputer = Model3D::Create("resource\\computer.dae");
	m3dComputer->SetPosition(-44, 11, -29);
	scene->AddModel3D(m3dComputer);
	auto m3dDesk = Model3D::Create("resource\\desk.dae");
	m3dDesk->SetPosition(-29, 11, -29);
	scene->AddModel3D(m3dDesk);
	auto m3dBookcase = Model3D::Create("resource\\bookcase.dae");
	m3dBookcase->SetPosition(-12, 11, -30);
	scene->AddModel3D(m3dBookcase);
	auto m3dTV = Model3D::Create("resource\\tv.dae");
	scene->AddModel3D(m3dTV);
	auto m3dFC = Model3D::Create("resource\\fc.dae");
	m3dFC->SetPositionZ(10);
	scene->AddModel3D(m3dFC);
	auto m3dBed = Model3D::Create("resource\\bed.dae");
	m3dBed->SetPosition(-28, 11, 11);
	scene->AddModel3D(m3dBed);
}

int main()
{
	auto app = PokeApp::Create("demo", 800, 600);
	
	if (app != nullptr)
	{
		auto scene = app->GetSceneManager();
		scene->SetFar(5000.0f);
		// 摄像机
		std::shared_ptr<Camera> camera(new Camera());
		camera->SetPosition(0.0f, 100.0f, 100.0f);
		camera->SetLookat(0.0f, 0.0f, -20.0f);
		scene->SetMainCamera(camera);
		
		LoadScene();
		app->GetAudioSystem()->Play("resource\\bgm.wav");
	
		app->SetFPS(60);
		app->Run();
	}
	
	return 0;
}