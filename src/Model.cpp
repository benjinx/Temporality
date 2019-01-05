#include "Model.hpp"

#include "Utils.hpp"
#include "Log.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

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

bool Model::Load(std::string filename)
{
	std::string ext = Utils::GetExtension(filename);
	bool binary = (ext == "glb");

	std::string fullPath;
	const auto& paths = Utils::GetResourcePaths();

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err, warn;

	bool loaded = false;
	for (auto& p : paths)
	{
		fullPath = p + filename;

		LogVerbose("Checking %s\n", fullPath);
		if (binary)
		{
			loaded = loader.LoadBinaryFromFile(&model, &err, &warn, fullPath);
		}
		else
		{
			loaded = loader.LoadASCIIFromFile(&model, &err, &warn, fullPath);
		}

		if (loaded) break;
	}

	if (!loaded)
	{
		LogError("Fail to load model, '%s'\n", filename);
		return false;
	}

	LogLoad("Loading Mesh from %s\n", fullPath);
	LogVerbose("Model Generator %s\n", model.asset.generator);

	std::vector<std::shared_ptr<Texture>> textures;
	for (auto& texture : model.textures)
	{
		tinygltf::Image& image = model.images[texture.source];
		if (texture.sampler <= 0)
		{
			LogVerbose("Loading Texture from buffer (%d, %d, %d)", image.width, image.height, image.component);
			textures.push_back(std::make_shared<Texture>(image.image.data(), glm::ivec2(image.width, image.height), image.component));
		}
		else
		{
			textures.push_back(std::make_shared<Texture>(std::move((GLuint)texture.sampler), glm::ivec2(image.width, image.height)));
		}
	}

	std::vector<std::shared_ptr<Material>> materials;
	for (auto& material : model.materials)
	{
		auto mat = std::make_shared<Material>();

		auto& vals = material.values;
		auto it = vals.end();

		auto& addVals = material.additionalValues;
		auto addIt = addVals.end();

		it = vals.find("baseColorFactor");
		if (it != vals.end() && !it->second.number_array.empty())
		{
			const auto& c = it->second.ColorFactor();
			mat->SetDiffuse(glm::make_vec4(it->second.ColorFactor().data())); // BaseColorFactor
		}

		it = vals.find("baseColorTexture");
		if (it != vals.end())
		{
			mat->SetDiffuseMap(std::make_shared<Texture>(textures[it->second.TextureIndex()]));
		}

		it = vals.find("metallicFactor");
		if (it != vals.end())
		{
			mat->SetMetallic((float)it->second.Factor());
		}

		it = vals.find("roughnessFactor");
		if (it != vals.end())
		{
			mat->SetRoughness((float)it->second.Factor());
		}

		it = vals.find("metallicRoughnessTexture");
		if (it != vals.end())
		{
			mat->SetMetallicRoughnessMap(std::make_shared<Texture>(textures[it->second.TextureIndex()]));
		}

		addIt = addVals.find("normalTexture");
		if (addIt != addVals.end())
		{
			mat->SetNormalMap(std::make_shared<Texture>(textures[it->second.TextureIndex()]));
			mat->SetNormalScale(addIt->second.Factor());
		}

		addIt = addVals.find("emissiveFactor");
		if (it != vals.end() && !it->second.number_array.empty())
		{
			const auto& c = it->second.ColorFactor();
			mat->SetEmissive(glm::make_vec3(it->second.ColorFactor().data()));
		}

		addIt = addVals.find("emissiveTexture");
		if (addIt != addVals.end())
		{
			mat->SetEmissiveMap(std::make_shared<Texture>(textures[it->second.TextureIndex()]));
		}

		addIt = addVals.find("occlusionTexture");
		if (addIt != addVals.end())
		{
			mat->SetOcclusionMap(std::make_shared<Texture>(textures[it->second.TextureIndex()]));
			mat->SetOcclusionStrength(addIt->second.Factor());
		}

		for (const auto& val : material.values)
		{
			LogVerbose("Material value %s\n", val.first);
		}

		for (const auto& val : material.additionalValues)
		{
			LogVerbose("Material additional value %s\n", val.first);
		}

		materials.push_back(std::move(mat));
	}

	if (materials.empty())
	{
		LogWarn("No Materials found, adding default\n");
		materials.push_back(std::make_shared<Material>());
	}

	std::vector<GLuint> vbos;

	const auto& scene = model.scenes[model.defaultScene];
	for (int id : scene.nodes)
	{
		auto& node = model.nodes[id];
		if (node.mesh < 0)
		{
			continue;
		}

		auto& mesh = model.meshes[node.mesh];
		for (size_t pInd = 0; pInd < mesh.primitives.size(); ++pInd)
		{
			auto& primitive = mesh.primitives[pInd];
			auto& indexAccessor = model.accessors[primitive.indices];

			GLuint vao;
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			{
				auto& bufferView = model.bufferViews[indexAccessor.bufferView];
				auto& buffer = model.buffers[bufferView.buffer];

				GLuint vbo;
				glGenBuffers(1, &vbo);
				vbos.push_back(vbo);

				glBindBuffer(bufferView.target, vbo);
				glBufferData(bufferView.target, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);
			}

			for (auto& attrib : primitive.attributes)
			{
				auto& accessor = model.accessors[attrib.second];
				auto& bufferView = model.bufferViews[accessor.bufferView];
				auto& buffer = model.buffers[bufferView.buffer];
				int byteStride = accessor.ByteStride(bufferView);

				LogVerbose("Attribute %s\n", attrib.first);

				GLuint vbo;
				glGenBuffers(1, &vbo);
				vbos.push_back(vbo);

				glBindBuffer(bufferView.target, vbo);
				glBufferData(bufferView.target, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);

				GLint size = (accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type);

				GLint vaa = -1;
				if (attrib.first.compare("POSITION") == 0)
				{
					vaa = AttributeID::POSITION;
				}

				if (attrib.first.compare("NORMAL") == 0)
				{
					vaa = AttributeID::NORMAL;
				}

				if (attrib.first.compare("TEXCOORD_0") == 0)
				{
					vaa = AttributeID::TEXCOORD;
				}

				if (attrib.first.compare("TANGENT") == 0)
				{
					vaa = AttributeID::TANGENT;
				}

				if (vaa > -1)
				{
					glEnableVertexAttribArray(vaa);
					glVertexAttribPointer(vaa, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, (void*)accessor.byteOffset);
				}
				else
				{
					LogWarn("Ignoring attribute %s\n", attrib.first);
				}
			}

			_mMeshes.push_back(new Mesh(vao,
										(GLenum)primitive.mode,
										(GLsizei)indexAccessor.count,
										(GLenum)indexAccessor.componentType,
										(GLsizei)indexAccessor.byteOffset,
										materials[(primitive.material < 0 ? 0 : primitive.material)]));
		}
	}

	glBindVertexArray(0);

	for (auto vbo : vbos)
	{
		glDeleteBuffers(1, &vbo);
	}

	_mLoaded = true;

	return _mLoaded;
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