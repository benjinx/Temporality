#include "Mesh.hpp"

#include "Camera.hpp"
#include "Shader.hpp"
#include "Material.hpp"

Mesh::Mesh(GLuint vao, GLenum mode, GLsizei count, GLenum type, GLsizei offset, std::shared_ptr<Material> material)
	: _mVAO(vao)
	, _mMode(mode)
	, _mCount(count)
	, _mType(type)
	, _mOffset(offset)
	, _mMaterial(material)
{ }

void Mesh::Render(Shader * shader, glm::mat4 modelMat)
{
	shader->Use();

    const auto& view = Camera::Inst().GetViewMat();
    const auto& proj = Camera::Inst().GetProjectionMat();

	shader->SetMat4("modelMat", modelMat);
	shader->SetMat4("viewMat", view);
	shader->SetMat4("projMat", proj);

	glm::mat4 mvp = proj * view * modelMat;
	shader->SetMat4("mvp", mvp);

    glm::vec3 camPos = Camera::Inst().GetCameraPos();
	glm::vec4 eyePos = glm::vec4(camPos.x, camPos.y, camPos.z, 1.0f);
	shader->SetVec4("eyePos", eyePos);

	if (_mMaterial != nullptr)
	{
		_mMaterial->Bind(shader);
	}

    glBindVertexArray(_mVAO);
    //glDrawArrays(GL_TRIANGLES, 0, _mCount);
	glDrawElements(_mMode, _mCount, _mType, (char *)0 + _mOffset);
    glBindVertexArray(0);
}