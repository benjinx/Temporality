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
    app->AddShader("skybox", new Shader({
        "shaders/skybox.vert",
        "shaders/skybox.frag" }));

    // Scene Objs

    ////////////////////////////////////////////////
    // Skybox
    ////////////////////////////////////////////////



    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // Pass in position at loc 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> faces = {
        "images/skyboxes/demo/right.jpg",
        "images/skyboxes/demo/left.jpg",
        "images/skyboxes/demo/top.jpg",
        "images/skyboxes/demo/bottom.jpg",
        "images/skyboxes/demo/front.jpg",
        "images/skyboxes/demo/back.jpg",};

    cubemapTexture = LoadCubemap(faces);

    ////////////////////////////////////////////////

    auto cube = Load("models/Primitives/pCube.glb");



    // UI
    DevUI::Start();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);

    App* app = App::Inst();
}

void GameScene::Render()
{
    App* app = App::Inst();

    //glDepthMask(GL_FALSE);
    auto skybox = app->GetShader("skybox");
    skybox->Use();
    glm::mat4 view = glm::mat3(app->GetCurrentCamera()->GetView());
    glm::mat4 proj = app->GetCurrentCamera()->GetProjection();
    skybox->SetMat4("viewMat", view);
    skybox->SetMat4("projMat", proj);

    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    //glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Render everything else after
    Scene::Render();
}

unsigned int GameScene::LoadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // This will iterate through all cube map targets by doing + i, normally they are
    // GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
    // GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Front
    // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Back
    int width, height, nrChannels;
    unsigned char* data;
    int i = 0;

    const auto& paths = Utils::GetResourcePaths();
    for (auto& f : faces)
    {
        for (auto& p : paths)
        {
            std::string fullFilename = p + "/" + f;

            data = stbi_load(fullFilename.c_str(), &width, &height, &nrChannels, 0);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
                LogLoad("Cubemap texture Loaded at path: %s\n", fullFilename.c_str());
                break;
            }
            else
            {
                LogWarn("Cubemap texture failed to load at path: %s\n", fullFilename.c_str());
                stbi_image_free(data);
            }
        }
        i++;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}