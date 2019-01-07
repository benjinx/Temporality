#include "GameScene.hpp"

void GameScene::Start()
{
	// Log Test
	printf("\n");
	LogInfo("Info\n");
	LogWarn("Warning\n");
	LogError("Error\n");
	LogPerf("Perf\n");
	LogVerbose("Verb\n");
	LogLoad("Load\n");

	// Camera
	auto camera = new Camera();
	_mGameObjects.emplace("Camera", camera);
	App::Inst()->SetCurrentCamera(camera);

	_mGameObjects["Camera"]->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));

	// Object setup
	printf("\nLoading Materials\n");

	// Scene Objs
	//_mGameObjects.emplace("Light", new GameObject("models/Primitives/pCube.obj"));
	//_mGameObjects.emplace("Cube", new GameObject("models/Primitives/pCube.obj"));
	//_mGameObjects.emplace("bulbs", new GameObject("models/bulbasaur.obj"));
	_mGameObjects.emplace("helmet", new GameObject("/models/DamagedHelmet.glb"));

	// Initialize Objs
	//_mGameObjects["Light"]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

	//_mGameObjects["Cube"]->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
	//_mGameObjects["Cube"]->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));

	//_mGameObjects["bulbs"]->SetPosition(glm::vec3(0.5f, 0.5f, 0.5f));
	//_mGameObjects["bulbs"]->SetRotation(glm::vec3(0.0f, -90.0f, 0.0f));

	// Shaders
	printf("\nLoading Shaders\n");

	App* app = App::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThru.vert",
		"shaders/passThru.frag" }));

	app->AddShader("advLighting", new Shader({
		"shaders/advLighting.vert",
		"shaders/advLighting.frag" }));

	//_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	//_mGameObjects["Cube"]->SetShader(app->GetShader("advLighting"));
	//_mGameObjects["bulbs"]->SetShader(app->GetShader("advLighting"));
	_mGameObjects["helmet"]->SetShader(app->GetShader("passThru"));

	// UI
	DevUI::Start();
	DevUI::RegisterOptionsFunc([this]() {
		ImGui::Checkbox("Enable Directional Light", &_mDirLight);
		ImGui::Checkbox("Enable Point Light", &_mPointLight);
		ImGui::Checkbox("Enable Spot Light", &_mSpotLight);
	});

	

	// Camera
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	//Camera::Inst().Init(cameraPos, cameraTarget);

	// Load lua script
	//_mScriptHost.Load();

	// Physics
	PhysicsStart();


	// Test 2D Mesh
	mesh = Utils::Get2DMesh(
		{ 0, 0, app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight() },
		{ 0, 1, 1, 0 });

	app->AddShader("ui", new Shader({
		"shaders/ui.vert",
		"shaders/ui.frag" }));


	//test mesh2d
	Shader* ui = app->GetShader("ui");
	ui->Use();
	glm::mat4 proj2d = glm::ortho(0.f, (float)app->GetWindow()->GetWidth(),
		0.f, (float)app->GetWindow()->GetHeight());
	app->GetShader("ui")->SetMat4("proj", proj2d);
}

void GameScene::PhysicsStart()
{
	// Testing Physics System Currently
	//glm::vec3 pos(2.0f, 2.0f, 2.0f);
	//Cube cube;
	//cube.SetPosition(pos);
	//cube.Update();
}

void GameScene::PhysicsUpdate(float dt)
{
	//
	//_mScene.GetGameObjects()["Sphere"]->Update(dt);
}

void GameScene::Update(float dt)
{
	Scene::Update(dt);

	

	_mGameObjects["helmet"]->SetRotation(_mGameObjects["helmet"]->GetRotation() * glm::angleAxis(glm::radians(0.0001f), glm::vec3(0.0f, 1.0f, 0.0f)));
	//_mGameObjects["helmet"]->SetRotation(glm::angleAxis(glm::radians(170.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	//LogInfo("Scale: %f, %f, %f\n", _mGameObjects["helmet"]->GetScale().x, _mGameObjects["helmet"]->GetScale().y, _mGameObjects["helmet"]->GetScale().z);

	// Get the application for ease.
	App* app = App::Inst();

	// Get reference to each shader
	Shader* passThru = app->GetShader("passThru");
	Shader* advLighting = app->GetShader("advLighting");

	passThru->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	passThru->SetVec3("passColor", lightColor);

	advLighting->Use();

	advLighting->SetVec3("lightColor", lightColor);

	//glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	//advLighting->SetVec3("lightVec", lightPos);

	//Camera::Inst().Update(dt);
	
	//PhysicsUpdate(dt);
}

void GameScene::Render()
{
	Scene::Render();

	// Render 2D Mesh below

	//glClear(GL_DEPTH_BUFFER_BIT);
	//App* app = App::Inst();
	//Shader* ui = app->GetShader("ui");
	//mesh->Render(ui, glm::mat4(1));
}