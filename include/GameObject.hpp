#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <Config.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

class Shader;
class Model;

//
class GameObject
{
public:

	GameObject();
	GameObject(glm::vec3 position);
	GameObject(std::string filename);
	virtual ~GameObject();

	virtual void Update(const float dt);
	void Render();
	void InitAxis();
	void DrawAxis();

	void SetShader(Shader* shader);

	void SetParent(GameObject* parent) { _mParent = parent; }
	GameObject* GetParent() { return _mParent; }
	void AddChild(GameObject* child) { _mChildren.push_back(child); }

	// Build Model Matrix
	glm::mat4 GetModelMatrix()
	{
		_mModelMatrix = glm::mat4(1);
		_mModelMatrix = glm::translate(_mModelMatrix, _mPosition);
		_mModelMatrix *= glm::mat4_cast(_mRotation);
		_mModelMatrix = glm::scale(_mModelMatrix, _mScale);
		return _mModelMatrix;
	}

	//// LOCAL
	// Local Transform
	void SetTransform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	{
		_mPosition = position;
		_mRotation = rotation;
		_mScale = scale;
	}

	glm::mat4 GetTransform() { return GetModelMatrix(); }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetPosition(glm::vec3 position) {
		_mPosition = position;
	}

	glm::vec3 GetPosition() const { return _mPosition; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetRotation(glm::quat rotation) {
		_mRotation = rotation;
	}

	glm::quat GetRotation() const { return _mRotation; }

	// Remember matrix order is Translate (Position), Rotate, Scale
	void SetScale(glm::vec3 scale) {
		_mScale = scale;
	}

	glm::vec3 GetScale() const { return _mScale; }
	//// END LOCAL

	//// WORLD
	// World Transform
	glm::mat4 GetWorldTransform()
	{
		if (GetParent())
		{
			return (GetParent()->GetTransform() * GetTransform());
		}

		return GetTransform();
	}

	glm::vec3 GetWorldPosition()
	{
		if (GetParent())
		{
			return (GetParent()->GetPosition() + GetPosition());
		}

		return GetPosition();
	}

	glm::quat GetWorldRotation()
	{
		if (GetParent())
		{
			return (GetParent()->GetRotation() * GetRotation());
		}

		return GetRotation();
	}

	glm::vec3 GetWorldScale()
	{
		if (GetParent())
		{
			return (GetParent()->GetScale() * GetScale());
		}

		return GetScale();
	}
	//// END WORLD
	
protected:

	// Model
	glm::mat4 _mModelMatrix = glm::mat4();

	// 
	glm::vec3 _mPosition = glm::vec3(0.0f),
			  _mScale = glm::vec3(1.0f);
	glm::quat _mRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	// Axis Rendering
	GLuint _mVAO;
	GLuint _mVBO[2];

	// Gobjs Shader
	Shader* _mShader;

	// Model
	Model* _mModel = nullptr;

	// Parent
	GameObject* _mParent = nullptr;

	// Children
	std::vector<GameObject*> _mChildren;
};
#endif // GAMEOBJECT_HPP
