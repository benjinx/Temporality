#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    // Object setup
    printf("\nLoading Models/Materials\n");

    // Camera
    auto camera = new Camera();
    Scene::AddGameObject("Camera", camera);
    Scene::GetGameObject("Camera")->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Scene Objs
    Scene::Load("models/logo/logo.glb");

    // Initialize Objs
    auto logo = Scene::GetGameObject("Logo");
    Scene::GetGameObject("Logo")->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    Scene::GetGameObject("Logo")->SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruTexture.vert",
        "shaders/passThruTexture.frag"
        }));

    Scene::GetGameObject("Logo")->SetShader(app->GetShader("passThru"));

    // UI
    DevUI::Start();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}
