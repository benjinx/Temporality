#ifndef CAMERA_H
#define CAMERA_H

#include <Config.hpp>
#include <GameObject.hpp>
#include <Math.hpp>

enum Direction
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Window;

class Camera : public GameObject
{
public:

	enum Mode {
		Perspective,
		Orthographic,
	};

	//DISALLOW_COPY_AND_ASSIGN(Camera);
	
	Camera();

	virtual ~Camera() = default;

	glm::mat4 GetView() const;

	glm::mat4 GetProjection() const;

	void SetMode(Mode mode);

	Mode GetMode() const { return _mMode; }

	void SetAspect(float aspect);

	void SetAspect(const glm::vec2& size);

	inline float GetAspect() const { return _mAspect; }

	void SetViewport(float left, float right, float bottom, float top);

	void SetViewport(const glm::vec4& viewport);

	void SetClip(const glm::vec2& clip);

	inline glm::vec2 GetClip() const { return _mClip; }

	void SetForward(const glm::vec3& forward);

	glm::vec3 GetForward() const;

	void SetLookAt(const glm::vec3& point);

	void HandleMovement(Direction dir, float dt);

	void HandleRotation(float xoffset, float yoffset);

	//virtual void Update(const float dt) override;

private:

	Mode _mMode = Mode::Perspective;

	glm::vec2 _mClip = glm::vec2(0.1f, 10000.0f);

	glm::vec3 _mUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float _mYaw = -90.0f,
		_mPitch = 0.0f;

	// Perspective
	float _mFov = glm::radians(45.0f);

	float _mAspect = 16.0f / 9.0f; // 16:9

	// Orthographic
	glm::vec4 _mViewport = glm::vec4(0.0f, 1280.0f, 720.0f, 0.0f);

	/*static Camera& Inst()
    {
		if (!_sInst) {
			_sInst = new Camera();
		}
        return *_sInst;
    }

	static void Delete() {
		if (_sInst) {
			delete _sInst;
			_sInst = nullptr;
		}
	}

    ~Camera() = default;

	Camera();

    void Init(glm::vec3 cameraPos, glm::vec3 cameraTarget);

    void Update(float dt);

    glm::mat4 GetViewMat() { return _mViewMat; }
    glm::mat4 GetProjectionMat() { return _mProjectionMat; }
    glm::vec3 GetCameraPos() { return _mPosition; }
    void SetCameraPos(glm::vec3 position) { _mPosition = position; }
    glm::vec3 GetCameraVelocity() { return _mVelocity; }
    void SetCameraVelocity(glm::vec3 velocity) { _mVelocity = velocity; }
	glm::vec2 GetResolution();
	glm::vec3 GetCameraForward() { return _mForward; }

	void HandleRotation(float x, float y);
	void HandleMovement(Direction dir, float dt);
	void HandleFoV(float xoffset, float yoffset);

private:

	static Camera * _sInst;

    Camera() = default;

    glm::mat4 _mViewMat, 
			  _mProjectionMat;
			  
    glm::vec3 _mPosition,
			  _mVelocity,
			  _mAcceleration,
			  _mForce,
			  _mForward,
			  _mRight,
			  _mUp,
			  _mTarget,
			  _mWorldUp;

	float _mFoV;
	float _mYaw = -90.0f, 
	      _mPitch = 0.0f;
	float _mMovementSpeed = 2.5f;
	bool  _mFirstMouse;
	*/
};

#endif // CAMERA_H
