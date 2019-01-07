#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <unordered_map>

class Shader;
class Model;

// Need to make sure to use transform data for modelMatrix.
struct Transform
{
	glm::vec3 position = glm::vec3(0.0f),
			  scale = glm::vec3(1.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};

//
class GameObject
{
public:

	GameObject();
	GameObject(glm::vec3 position);
	GameObject(std::string filename);
	virtual ~GameObject();

	void Update(const float dt);
	void Render();


	// Local Transform
	Transform GetTransform() { return _mTransform; }
	void SetTransform(Transform transform) { _mTransform = transform; }
	void SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	{
		_mTransform.position = position;
		_mTransform.rotation = rotation;
		_mTransform.scale = scale;
	}

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetPosition(glm::vec3 position) { 
		_mTransform.position = position;
	}

	glm::vec3 GetPosition() const { return _mTransform.position; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetRotation(glm::quat rotation) { 
		_mTransform.rotation = rotation;
	}
	glm::quat GetRotation() const { return _mTransform.rotation; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetScale(glm::vec3 scale) { 
		_mTransform.scale = scale;
	}
	glm::vec3 GetScale() const { return _mTransform.scale; }

	// World Transform
	Transform GetWorldTransform() { return _mTransform; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetWorldPosition(glm::vec3 position)
	{
		//_mWorldTransform.position = position;
		_mTransform.position = position;
	}

	glm::vec3 GetWorldPosition() const { return _mTransform.position; /*return _mWorldTransform.position;*/ }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetWorldRotation(glm::quat rotation)
	{
		//_mWorldTransform.rotation = rotation;
		_mTransform.rotation = rotation;
	}

	glm::quat GetWorldRotation() const { return _mTransform.rotation; /*return _mWorldTransform.rotation;*/ }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetWorldScale(glm::vec3 scale)
	{
		//_mWorldTransform.scale = scale;
		_mTransform.scale = scale;
	}

	glm::vec3 GetWorldScale() const { return _mTransform.scale; /*return _mWorldTransform.scale;*/ }

	// Build Model Matrix
	glm::mat4 GetModelMatrix()
	{
		_mModelMatrix = glm::mat4(1);
		_mModelMatrix = glm::translate(_mModelMatrix, _mTransform.position);
		_mModelMatrix *= glm::mat4_cast(_mTransform.rotation);
		_mModelMatrix = glm::scale(_mModelMatrix, _mTransform.scale);
		return _mModelMatrix;
	}

	GameObject* GetChild(std::string name) { 
		if (_mChildren.find(name) != _mChildren.end())
			return _mChildren[name];

		return nullptr;
	}

	void InitAxis();
	void DrawAxis();

	void SetShader(Shader* shader);
	
protected:
	// Transform
	Transform _mTransform;

	// Model
	glm::mat4 _mModelMatrix = glm::mat4();

	// Children
	std::unordered_map<std::string, GameObject*> _mChildren;

	// Axis Rendering
	GLuint _mVAO;
	GLuint _mVBO[2];

	// Gobjs Shader
	Shader* _mShader;

	// Model
	Model* _mModel = nullptr;
};

// GameObject needs to know which components are attached to each gameobject.

#endif // GAMEOBJECT_HPP
