#include "Scene.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <DevUI.hpp>
#include <glTF2.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <MeshComponent.hpp>
#include <Shader.hpp>
#include <Utils.hpp>

#include <imgui/imgui.h>

void Scene::Update(float dt)
{
    GameObject::Update(dt);

    // Use our default shaders and set the color & light position.
    App* app = App::Inst();
    Shader* defaultLighting = app->GetShader("defaultLighting");

    defaultLighting->Use();
    glm::vec3 defaultLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    defaultLighting->SetVec3("lightColor", defaultLightColor);

    glm::vec4 defaultLightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    defaultLighting->SetVec4("lightPosition", defaultLightPosition);
}

void Scene::Render()
{
    GameObject::Render();

    if (DevUI::showAxis)
    {
        RenderAxis();

        // Render the childrens
        for (auto& gameObject : _mChildren)
        {
            RenderAxis();
        }
    }
}

bool Scene::LoadScene(std::string filename)
{
    std::vector<std::unique_ptr<GameObject>> loadedGobjs = glTF2::LoadSceneFromFile(filename);

    for (int i = 0; i < loadedGobjs.size(); ++i)
    {
        AddGameObject(std::move(loadedGobjs[i]));

        // This will work if we have names from parsing.
        //AddGameObject(loadedGobjs[i]->GetName(), std::move(loadedGobjs[i]));
    }

    if (loadedGobjs.empty())
    {
        return false;
    }

    return true;
}

GameObject* Scene::AddGameObject(std::string name, std::unique_ptr<GameObject> gameObject)
{
    _mChildren.push_back(std::move(gameObject));
    _mChildren.back()->SetName(name);
    _mChildren.back()->SetParent(this);
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject()
{
    _mChildren.push_back(std::make_unique<GameObject>());
    _mChildren.back()->SetParent(this);
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject(std::string name)
{
    _mChildren.push_back(std::make_unique<GameObject>());
    _mChildren.back()->SetName(name);
    _mChildren.back()->SetParent(this);
    return _mChildren.back().get();
}

GameObject* Scene::AddGameObject(std::unique_ptr<GameObject> gobj)
{
    _mChildren.push_back(std::move(gobj));
    _mChildren.back()->SetParent(this);
    return _mChildren.back().get();
}


void Scene::CreateSkybox(std::vector<std::string> faces)
{
    auto skybox = std::make_unique<Skybox>();

    skybox->LoadCubemap(faces);

    _mSkybox = skybox.get();

    AddGameObject(std::move(skybox));
}

void Scene::Options()
{
    ImGui::Checkbox("Show GameObject Axis", &DevUI::showAxis);
}