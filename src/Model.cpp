#include "Model.hpp"

#include <App.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Utils.hpp>

#include <nlohmann/json.hpp>

#include <stb/stb_image.h>

Model::Model(std::shared_ptr<Mesh> mesh)
{
    _mMeshes.push_back(mesh);
}

Model::Model(std::vector<std::shared_ptr<Mesh>> meshes)
{
    _mMeshes = meshes;
}

void Model::Render(Shader* shader, glm::mat4 modelMatrix)
{
    for (auto& mesh : _mMeshes)
    {
        mesh->Render(shader, modelMatrix);
    }
}