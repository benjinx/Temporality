#include "Model.hpp"

#include "Utils.hpp"
#include "Log.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

Model::Model(std::string filename)
{
	LoadObj(filename);
}

Model::~Model()
{
	for (Mesh* mesh : _mMeshes)
	{
		delete mesh;
	}
}

void Model::Render(Shader* shader, glm::mat4 modelMatrix)
{
	for (Mesh* mesh : _mMeshes)
	{
		mesh->Render(shader, modelMatrix);
	}
}

void Model::LoadObj(std::string filename)
{
	const auto& paths = Utils::GetResourcePaths();

	LogInfo("Loading: [%s]\n", filename);

	Assimp::Importer importer;
	for (const std::string& p : paths) {
		std::string fullFilename = p + "/" + filename;

		const aiScene* scene = importer.ReadFile(fullFilename,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_FlipUVs);

		if (!scene) continue;

		LogInfo("Loaded:  [%s]\n", fullFilename);

		std::string dirname = Utils::GetDirname(fullFilename) + "/";

		std::vector<Mesh*> meshes;

		// ProcessNode
		ProcessNode(scene->mRootNode, scene, meshes, dirname);

		AddMesh(meshes[0]);

		return;
	}

	LogError("Failed to load [%s]", filename);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, meshes, dirname));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, meshes, dirname);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh*>& meshes, std::string dirname)
{
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<Texture> textures;

	// Indicies
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//vertices.reserve(indices.size());
	//normals.reserve(indices.size());
	//texCoords.reserve(indices.size());
	//tangents.reserve(indices.size());
	//bitangents.reserve(indices.size());

	//for (auto& i : indices)
	//{
	//	auto& temp = vertices[i];
	//	std::vector<glm::vec3> newVert;
	//	newVert.push_back(temp);
	//}

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// Vertices
		glm::vec3 vertice(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertices.push_back(vertice);

		// Normals
		if (mesh->HasNormals())
		{
			glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			normals.push_back(normal);
		}

		// TexCoords
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texCoord(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			texCoords.push_back(texCoord);
		}
		else
			texCoords.push_back(glm::vec2(0.0f, 0.0f));

		// Tangents
		if (mesh->HasTangentsAndBitangents())
		{
			glm::vec3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			tangents.push_back(tangent);

			glm::vec3 bitangent(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
			bitangents.push_back(bitangent);
		}
	}

	Mesh* newMesh = new Mesh(vertices, normals, texCoords, tangents, bitangents);
	Material* newMat = nullptr;

	// Materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Ambient
		std::string ambientMap = GetMaterialTextureName(material, aiTextureType_AMBIENT, dirname);

		// Diffuse
		std::string diffuseMap = GetMaterialTextureName(material, aiTextureType_DIFFUSE, dirname);

		// Specular
		std::string specularMap = GetMaterialTextureName(material, aiTextureType_SPECULAR, dirname);

		// Normal
		std::string normalMap = GetMaterialTextureName(material, aiTextureType_NORMALS, dirname);

		aiColor4D aiAmb;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &aiAmb);
		float amb[3] = { aiAmb.r, aiAmb.g, aiAmb.b };
		glm::vec3 ambient = glm::vec3(amb[0], amb[1], amb[2]);

		aiColor4D aiDiff;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &aiDiff);
		float diff[3] = { aiDiff.r, aiDiff.g, aiDiff.b };
		glm::vec3 diffuse = glm::vec3(diff[0], diff[1], diff[2]);

		aiColor4D aiSpec;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &aiSpec);
		float spec[3] = { aiSpec.r, aiSpec.g, aiSpec.b };
		glm::vec3 specular = glm::vec3(spec[0], spec[1], spec[2]);

		aiColor4D aiShininess;
		aiGetMaterialColor(material, AI_MATKEY_SHININESS, &aiShininess);
		glm::vec3 shininess(aiShininess.r, aiShininess.g, aiShininess.b);

		// Notes:
		// Remove dissolve from material it's pointless
		// Add all the other materials, Albedo, Metallic, roughness, AO?, etc


		newMat = new Material(ambient, diffuse, specular, aiShininess.r, std::make_shared<Texture>(ambientMap), std::make_shared<Texture>(diffuseMap), std::make_shared<Texture>(specularMap), std::make_shared<Texture>(normalMap));
		newMesh->SetMaterial(newMat);
	}

	return newMesh;
}

std::string Model::GetMaterialTextureName(aiMaterial* material, aiTextureType type, std::string dirname)
{
	std::string texName;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		texName = dirname + str.C_Str();
	}
	return texName;
}