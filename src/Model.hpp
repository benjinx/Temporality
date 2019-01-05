#ifndef MODEL_HPP
#define MODEL_HPP

#include <Config.hpp>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Shader;
class Mesh;

class Model
{
public:
	Model(std::string filename);
	~Model();

	void Render(Shader* shader, glm::mat4 modelMatrix);

	void AddMesh(Mesh* mesh) { _mMeshes.push_back(mesh); }

	Mesh* GetMesh(int index = 0) { return _mMeshes[index]; }
	unsigned int GetNumMeshes() { return (unsigned int)_mMeshes.size(); }

private:
	// Object loading
	void LoadObj(std::string filename);
	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname);
	std::string GetMaterialTextureName(aiMaterial* material, aiTextureType type, std::string dirname);

	// Mesh
	std::vector<Mesh*> _mMeshes;
};

#endif //MODEL_HPP