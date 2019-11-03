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

    // Light Source
    //Scene::Load("models/Primitives/pCube.glb");
    //auto Light = Scene::GetGameObject("cube");
    auto Light = Scene::AddGameObject();
    Light->SetName("Light");
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Scene Objs
    Scene::Load("models/Primitives/pPlane.glb");
    Scene::Load("models/Primitives/pSphere.glb");
    Scene::Load("models/Primitives/pCube.glb");
    Scene::Load("models/Primitives/pTorus.glb");

    // Initialize Objs
    auto Plane = Scene::GetGameObject("Plane");
    auto Sphere = Scene::GetGameObject("sphere");
    auto Cube = Scene::GetGameObject("cube");
    auto Torus = Scene::GetGameObject("Torus");
    
    Plane->SetPosition(glm::vec3(0.0f, -2.5f, 0.0f));
    Plane->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    Plane->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));

    Sphere->SetPosition(glm::vec3(1.5f, 0.0f, 2.0f));
    Sphere->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
    Sphere->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Cube->SetPosition(glm::vec3(-1.5f, -1.0f, 0.0f));
    Cube->SetRotation(glm::vec3(20.0f, 0.0f, 20.0f));
    Cube->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

    Torus->SetPosition(glm::vec3(1.0f, 1.0f, -1.0f));
    Torus->SetRotation(glm::vec3(-90.0f, -90.0f, 0.0f));
    Torus->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("basicLighting", new Shader({
        "shaders/basicLighting.vert",
        "shaders/basicLighting.frag" }));

    //Light->SetShader(app->GetShader("passThru"));
    Plane->SetShader(app->GetShader("basicLighting"));
    Sphere->SetShader(app->GetShader("basicLighting"));
    Cube->SetShader(app->GetShader("basicLighting"));
    Torus->SetShader(app->GetShader("basicLighting"));

    // UI
    DevUI::Start();

    // Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);

    // Set Shader values
    
    // Get the application for ease.
    App* app = App::Inst();

    // Get reference to each shader
    Shader* passThru = app->GetShader("passThru");
    Shader* basicLighting = app->GetShader("basicLighting");


    // Set Light Color
    passThru->Use();
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    passThru->SetVec3("passColor", lightColor);

    // Set Light Position
    basicLighting->Use();

    basicLighting->SetVec3("lightColor", lightColor);

    glm::vec4 lightPos = glm::vec4(Scene::GetGameObject("Light")->GetPosition(), 1.0f);
    basicLighting->SetVec4("lightVec", lightPos);

    glm::vec3 objColor = glm::vec3(UI::objectColor[0], UI::objectColor[1], UI::objectColor[2]);
    basicLighting->SetVec3("objectColor", objColor);

    // Rotate objects
    Scene::GetGameObject("sphere")->SetRotation(Scene::GetGameObject("sphere")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 0.0f, 1.0f)));
    Scene::GetGameObject("cube")->SetRotation(Scene::GetGameObject("cube")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
    Scene::GetGameObject("Torus")->SetRotation(Scene::GetGameObject("Torus")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
        
}