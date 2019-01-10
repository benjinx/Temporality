#include "GameScene.hpp"

#include "UI.hpp"

void GameScene::Start()
{
	// Object setup
	printf("\nLoading Models/Materials\n");

	// Camera
	auto camera = new Camera();
	_mGameObjects.emplace("Camera", camera);
	_mGameObjects["Camera"]->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	App::Inst()->SetCurrentCamera(camera);

	// Light Source
	_mGameObjects.emplace("Light", new GameObject("/models/Primitives/pSphere.glb"));

	_mGameObjects["Light"]->SetPosition(glm::vec3(-2.0f, -0.5f, 1.0f));
	_mGameObjects["Light"]->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	_mGameObjects["Light"]->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));

	// Initialize Objs
	_mGameObjects.emplace("Earth", new GameObject("/models/earth.glb"));
	_mGameObjects.emplace("Moon", new GameObject("/models/moon.glb"));

	_mGameObjects["Earth"]->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	_mGameObjects["Earth"]->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	_mGameObjects["Earth"]->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	_mGameObjects["Moon"]->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	_mGameObjects["Moon"]->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	_mGameObjects["Moon"]->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	_mGameObjects.emplace("helm", new GameObject("/models/DamagedHelmet.glb"));
	_mGameObjects["helm"]->SetPosition(glm::vec3(2.0f, 2.0f, 2.0f));

	// Shaders
	printf("\nLoading Shaders\n");

	App* app = App::Inst();
	app->AddShader("passThru", new Shader({
		"shaders/passThruColor.vert",
		"shaders/passThruColor.frag" }));

	app->AddShader("normalMapping", new Shader({
		"shaders/normalMapping.vert",
		"shaders/normalMapping.frag" }));

	_mGameObjects["Light"]->SetShader(app->GetShader("passThru"));
	_mGameObjects["Earth"]->SetShader(app->GetShader("normalMapping"));
	_mGameObjects["Moon"]->SetShader(app->GetShader("normalMapping"));
	_mGameObjects["helm"]->SetShader(app->GetShader("normalMapping"));

	// UI
	DevUI::Start();

	// Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameScene::Update(float dt)
{
	Scene::Update(dt);

	// Get the application for ease.
	App* app = App::Inst();

	// Get reference to each shader
	Shader* passThru = app->GetShader("passThru");
	Shader* normalMapping = app->GetShader("normalMapping");

	// Set Shader values

	// Set Light Color
	passThru->Use();
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	passThru->SetVec3("passColor", lightColor);

	// Set Light Position
	normalMapping->Use();

	normalMapping->SetVec3("lightColor", lightColor);
	glm::vec4 lightPos = glm::vec4(_mGameObjects["Light"]->GetPosition(), 1.0f);
	normalMapping->SetVec4("lightPos", lightPos);

	// Rotate objects
	//_mGameObjects["Earth"]->SetRotation(_mGameObjects["Earth"]->GetRotation() + glm::vec3(0.0f, 0.25f * dt, 0.0f));
	//_mGameObjects["Moon"]->SetRotation(_mGameObjects["Moon"]->GetRotation() + glm::vec3(0.0f, 0.5f * dt, 0.0f));

	/*const auto& earthPos = _mGameObjects["Earth"]->GetPosition();
	const auto& moonPos = _mGameObjects["Moon"]->GetPosition();

	_mAngle += 0.5f * dt;

	if (_mAngle > 360.0f)
		_mAngle = 0.0f;

	float radian = glm::radians(_mAngle);

	float radius = glm::distance(moonPos, earthPos);

	float x = earthPos.x + (radius * cosf(radian));
	float z = earthPos.z + (radius * sinf(radian));

	glm::vec3 newPos = glm::vec3(x, 0.0f, -z);
	
	_mGameObjects["Moon"]->SetPosition(newPos);*/
}
