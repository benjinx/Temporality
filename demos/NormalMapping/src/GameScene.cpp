#include "GameScene.hpp"

#include "UI.hpp"

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
    auto Light = Scene::AddGameObject();
    Light->SetName("Light");
    Light->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    // Initialize Objs
    Scene::Load("models/earth.glb");
    Scene::Load("models/moon.glb");
    Scene::Load("models/mars.glb");

    auto Earth = Scene::GetGameObject("Earth");
    auto Moon = Scene::GetGameObject("Moon");
    auto Mars = Scene::GetGameObject("Mars");

    Earth->SetPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    Earth->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    Earth->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));

    Moon->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
    Moon->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

    Mars->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
    Mars->SetScale(glm::vec3(1.2f, 1.2f, 1.2f));

    // Shaders
    printf("\nLoading Shaders\n");

    App* app = App::Inst();
    app->AddShader("passThru", new Shader({
        "shaders/passThruColor.vert",
        "shaders/passThruColor.frag" }));

    app->AddShader("normalMapping", new Shader({
        "shaders/normalMapping.vert",
        "shaders/normalMapping.frag" }));

    Earth->SetShader(app->GetShader("normalMapping"));
    Moon->SetShader(app->GetShader("normalMapping"));
    Mars->SetShader(app->GetShader("normalMapping"));

    // UI
    DevUI::Start();
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
    glm::vec4 lightPos = glm::vec4(Scene::GetGameObject("Light")->GetPosition(), 1.0f);
    normalMapping->SetVec4("lightPos", lightPos);

    glm::vec3 camPos = App::Inst()->GetCurrentCamera()->GetPosition();
    glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
    normalMapping->SetVec4("eyePos", eyePos);

    // Rotate objects
    Scene::GetGameObject("Earth")->SetRotation(Scene::GetGameObject("Earth")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.25f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    Scene::GetGameObject("Moon")->SetRotation(Scene::GetGameObject("Moon")->GetWorldRotation()
        * glm::angleAxis(glm::radians(-0.5f) * dt, glm::vec3(0.0f, 1.0f, 0.0f)));

    const auto& earthPos = Scene::GetGameObject("Earth")->GetPosition();
    const auto& moonPos = Scene::GetGameObject("Moon")->GetPosition();
    const auto& marsPos = Scene::GetGameObject("Mars")->GetPosition();

    _mAngle += 0.5f * dt;

    if (_mAngle > 360.0f)
        _mAngle = 0.0f;

    float radian = glm::radians(_mAngle);

    float radius = glm::distance(moonPos, earthPos);

    float x = earthPos.x + (radius * cosf(radian));
    float z = earthPos.z + (radius * sinf(radian));

    glm::vec3 newPos = glm::vec3(x, 0.0f, -z);
    
    Scene::GetGameObject("Moon")->SetPosition(newPos);

    _mMarsAngle += 0.5f * dt;

    if (_mMarsAngle > 360.0f)
        _mMarsAngle = 0.0f;

    float marsRadian = glm::radians(_mMarsAngle);

    float marsRadius = glm::distance(marsPos, glm::vec3(lightPos.x, lightPos.y, lightPos.z));

    float marsX = lightPos.x + (marsRadius * cosf(marsRadian));
    float marsZ = lightPos.z + (marsRadius * sinf(marsRadian));

    glm::vec3 newMarsPos = glm::vec3(marsX, 0.0f, -marsZ);

    //Scene::GetGameObject("Mars")->SetPosition(newMarsPos);
}
