#include "GameScene.hpp"

void GameScene::Start()
{
    Scene::Start();

    // Object setup
    printf("\nLoading Models/Materials\n");

    // Camera
    auto camera = new Camera();
    AddGameObject("Camera", camera);
    GetGameObject("Camera")->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    App::Inst()->SetCurrentCamera(camera);

    // Light Source
    auto Light = AddGameObject();
    Light->SetName("Light");
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Scene Objs
    Load("models/Primitives/pPlane.glb");
    Load("models/Primitives/pSphere.glb");
    Load("models/Primitives/pCube.glb");
    Load("models/Primitives/pTorus.glb");

    // Initialize Objs

    auto Plane = GetGameObject("Plane");
    auto Sphere = GetGameObject("sphere");
    auto Cube = GetGameObject("cube");
    auto Torus = GetGameObject("Torus");

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

    app->AddShader("advLighting", new Shader({
        "shaders/advLighting.vert",
        "shaders/advLighting.frag" }));

    Plane->SetShader(app->GetShader("advLighting"));
    Sphere->SetShader(app->GetShader("advLighting"));
    Cube->SetShader(app->GetShader("advLighting"));
    Torus->SetShader(app->GetShader("advLighting"));

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
    Shader* advLighting = app->GetShader("advLighting");

    // Set Light Color
    passThru->Use();
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    passThru->SetVec3("passColor", lightColor);

    // Set Light Position
    advLighting->Use();

    advLighting->SetVec3("lightColor", lightColor);
    glm::vec4 lightPos = glm::vec4(GetGameObject("Light")->GetPosition(), 1.0f);
    advLighting->SetVec4("lightVec", lightPos);

    glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
    glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
    advLighting->SetVec4("eyePos", eyePos);

    
    // Rotate objects
    GetGameObject("Sphere")->SetRotation(GetGameObject("Sphere")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(1.0f, 0.0f, 0.0f)));
    GetGameObject("cube")->SetRotation(GetGameObject("cube")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
    GetGameObject("Torus")->SetRotation(GetGameObject("Torus")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 0.0f, 1.0f)));

}
