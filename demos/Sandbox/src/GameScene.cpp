#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    LogTest();

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruTexture.vert",
        "shaders/passThruTexture.frag" }));

    app->AddShader("normalMapping", new Shader({
        "shaders/normalMapping.vert",
        "shaders/normalMapping.frag" }));

    app->AddShader("lightCasters", new Shader({
        "shaders/lightCasters.vert",
        "shaders/lightCasters.frag" }));

    // Scene Objs
    Load("models/DamagedHelm.glb");
    auto helm = GetGameObject("node_damagedHelmet_-6514");
    helm->SetPosition(glm::vec3(2.0f, 0.0f, 1.0f));
    helm->SetShader(app->GetShader("normalMapping"));

    Load("models/logo/logo.glb");
    auto logo = GetGameObject("Logo");
    logo->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    logo->SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    logo->SetShader(app->GetShader("passThru"));

    Load("models/defaultScene3.glb");
    //auto cube = GetGameObject("Cube");
    //cube->SetPosition(glm::vec3(-1.0f, -1.0f, -1.0f));
    //cube->SetShader(app->GetShader("passThru"));

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // UI
    DevUI::Start();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);

    App* app = App::Inst();
    auto norm = app->GetShader("normalMapping");

    norm->Use();

    norm->SetVec4("lightPos", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void GameScene::LogTest()
{
    // Log Test
    printf("\n");
    LogInfo("Info\n");
    LogWarn("Warning\n");
    LogError("Error\n");
    LogPerf("Perf\n");
    LogVerbose("Verb\n");
    LogLoad("Load\n");
}