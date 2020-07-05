#include "GameScene.hpp"

#include <stb/stb_image.h>

void GameScene::Start()
{
    Scene::Start();

    // Camera
    Camera * camera = (Camera *)AddGameObject("Camera", std::make_unique<Camera>());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();

    std::vector<std::string> faces = {
        "images/skyboxes/demo/right.jpg",
        "images/skyboxes/demo/left.jpg",
        "images/skyboxes/demo/top.jpg",
        "images/skyboxes/demo/bottom.jpg",
        "images/skyboxes/demo/front.jpg",
        "images/skyboxes/demo/back.jpg",
    };

    std::vector<std::string> faces2 = {
        "images/skyboxes/hw_lagoon/right.tga",
        "images/skyboxes/hw_lagoon/left.tga",
        "images/skyboxes/hw_lagoon/top.tga",
        "images/skyboxes/hw_lagoon/bottom.tga",
        "images/skyboxes/hw_lagoon/front.tga",
        "images/skyboxes/hw_lagoon/back.tga",
    };

    std::vector<std::string> faces3 = {
        "images/skyboxes/ame_nebula/right.tga",
        "images/skyboxes/ame_nebula/left.tga",
        "images/skyboxes/ame_nebula/top.tga",
        "images/skyboxes/ame_nebula/bottom.tga",
        "images/skyboxes/ame_nebula/front.tga",
        "images/skyboxes/ame_nebula/back.tga",
    };

    // Call skybox here
    CreateSkybox(faces);

    auto Cube = AddGameObject("Cube");
    auto CubeMesh = (MeshComponent*)Cube->AddComponent(std::make_unique<MeshComponent>());
    CubeMesh->SetShader(app->GetShader("defaultLighting"));

    if (CubeMesh->Load("models/Primitives/pCube.glb"))
    {
        Cube->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
        Cube->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
        //Cube->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // UI
    DevUI::Start();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}

void GameScene::Render()
{
    App* app = App::Inst();

    // Render everything else after
    Scene::Render();
}