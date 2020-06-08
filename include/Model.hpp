#ifndef MODEL_HPP
#define MODEL_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>
#include <Mesh.hpp>

#include <vector>
#include <memory>

class Shader;

class Model
{
public:
    Model(std::shared_ptr<Mesh> mesh);
    Model(std::vector<std::shared_ptr<Mesh>> meshes);

    virtual ~Model() = default;

    void Render(Shader* shader, glm::mat4 modelMatrix);

    void AddMesh(std::shared_ptr<Mesh> mesh) {
        _mMeshes.push_back(mesh);
    }

private:

    // Mesh
    std::vector<std::shared_ptr<Mesh>> _mMeshes;

};

#endif //MODEL_HPP