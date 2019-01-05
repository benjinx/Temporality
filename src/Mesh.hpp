#ifndef MESH_H
#define MESH_H

#include "Config.hpp"
#include "Light.hpp"

class Shader;
class Material;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texCoords;
};

class Mesh
{
public:
    /* Functions */
    Mesh(GLuint vao, GLenum mode, GLsizei count, GLenum type, GLsizei offset, std::shared_ptr<Material> material);

	void Render(Shader * shader, glm::mat4 modelMat);

    void SetMaterial(Material* material) { _mMaterial = material; }

    GLuint GetVAO() { return _mVAO; }

private:
    /* Render Data */
	GLuint _mVAO;
	GLenum	_mMode;
	GLsizei _mCount;
	GLenum _mType;
	GLsizei _mOffset;
	std::shared_ptr<Material> _mMaterial = nullptr;
};

#endif // MESH_H
