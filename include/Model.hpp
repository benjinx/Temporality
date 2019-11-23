#ifndef MODEL_HPP
#define MODEL_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <vector>

enum AttributeID : GLint
{
    POSITION = 0,
    NORMAL = 1,
    TEXCOORD = 2,
    TANGENT = 3,
    BITANGENT = 4,
    COLOR = 5,
};

class Shader;
class Mesh;

class Model
{
public:
    Model(std::vector<Mesh*> meshes);
    ~Model();

    void Render(Shader* shader, glm::mat4 modelMatrix);

    void AddMesh(Mesh* mesh) { _mMeshes.push_back(mesh); }

    Mesh* GetMesh(int index = 0) { return _mMeshes[index]; }
    unsigned int GetNumMeshes() { return (unsigned int)_mMeshes.size(); }

private:
    // Mesh
    std::vector<Mesh*> _mMeshes;

};

#endif //MODEL_HPP