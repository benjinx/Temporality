#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    LogTest();

    // Light Source
    auto Light = AddGameObject("Light");
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("normalMapping", new Shader({
        "shaders/normalMapping.vert",
        "shaders/normalMapping.frag" }));

    app->AddShader("lightCasters", new Shader({
        "shaders/lightCasters.vert",
        "shaders/lightCasters.frag" }));

    // Scene Objs
    auto helm = AddGameObject("helm");
    auto helmMesh = (MeshComponent*)helm->AddComponent(std::make_unique<MeshComponent>());
    helmMesh->SetShader(app->GetShader("normalMapping"));

    if (helmMesh->Load("models/DamagedHelm.glb"))
    {
        helm->SetPosition(glm::vec3(2.0f, 0.0f, 1.0f));
        helm->SetRotation(glm::vec3(0.0f, 9.5f, 9.5f));
    }

    auto cube = AddGameObject("Cube");
    auto cubeMesh = (MeshComponent*)cube->AddComponent(std::make_unique<MeshComponent>());
    cubeMesh->SetShader(app->GetShader("passThru"));

    //if (cubeMesh->Load("models/defaultScene2.glb"))
    //{
    //    cube->SetPosition(glm::vec3(-1.0f, -1.0f, -1.0f));
    //}

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

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    norm->Use();
    norm->SetVec3("lightColor", lightColor);
    glm::vec4 lightPos = glm::vec4(FindGameObject("Light")->GetPosition(), 1.0f);
    norm->SetVec4("lightPos", lightPos);

    auto color = app->GetShader("passThru");
    color->Use();
    color->SetVec3("passColor", glm::vec3(0.0f, 0.0f, 1.0f));
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