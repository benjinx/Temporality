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
    //App::Inst()->AddShader("reflection", new Shader({
    //    "shaders/reflection.vert",
    //    "shaders/reflection.frag" }));

    App::Inst()->AddShader("refraction", new Shader({
        "shaders/refraction.vert",
        "shaders/refraction.frag" }));


    std::vector<std::string> faces = {
        "images/skyboxes/demo/right.jpg",
        "images/skyboxes/demo/left.jpg",
        "images/skyboxes/demo/top.jpg",
        "images/skyboxes/demo/bottom.jpg",
        "images/skyboxes/demo/front.jpg",
        "images/skyboxes/demo/back.jpg",
    };

    // Call skybox here
    CreateSkybox(faces);
    //GetSkybox()->SetShader(app->GetShader("reflective"));

    auto Cube = AddGameObject("Cube");
    auto CubeMesh = (MeshComponent*)Cube->AddComponent(std::make_unique<MeshComponent>());
    //CubeMesh->SetShader(app->GetShader("defaultLighting"));
    //CubeMesh->SetShader(app->GetShader("reflection"));
    CubeMesh->SetShader(app->GetShader("refraction"));

    if (CubeMesh->Load("models/Primitives/pCube.glb"))
    {
        Cube->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
        //Cube->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
        //Cube->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // UI
    DevUI::Start();

    // UI
    DevUI::RegisterOptionsFunc([this]() {
        ImGui::Checkbox("Enable Reflective", &_mReflection);
        ImGui::Checkbox("Enable Refractive", &_mRefraction);
    });
}

void GameScene::Update(float dt)
{
    App* app = App::Inst();

    if (_mReflection)
    {
        //FindGameObject("Cube")->
        //CubeMesh->SetShader(app->GetShader("reflection"));
    }
    else if (_mRefraction)
    {
        //CubeMesh->SetShader(app->GetShader("refraction"));
    }
    else
    {
        //CubeMesh->SetShader(app->GetShader("defaultLighting"));
    }

    Scene::Update(dt);
}

void GameScene::Render()
{
    App* app = App::Inst();

    glm::vec3 camPos = app->GetCurrentCamera()->GetPosition();
    glm::vec3 eyePos = glm::vec3(camPos.x, camPos.y, camPos.z);

    //if (_mReflection)
    //{
    //    auto refection = app->GetShader("reflection");
    //    refection->Use();
    //    refection->SetVec3("eyePos", eyePos);

    //    // Use this after using an objects shader 
    //    // to get the skybox info
    //    GetSkybox()->PreRender();
    //}
    //else if (_mRefraction)
    if (_mRefraction)
    {
        auto refraction = app->GetShader("refraction");
        refraction->Use();
        refraction->SetVec3("eyePos", eyePos);

        // Use this after using an objects shader 
        // to get the skybox info
        GetSkybox()->PreRender();
    }

    // Render everything else after
    Scene::Render();
}