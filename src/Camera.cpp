#include "Camera.hpp"

#include <App.hpp>
#include <Window.hpp>

//Camera * Camera::_sInst = nullptr;

Camera::Camera()
{
	const glm::ivec2& size = glm::ivec2(App::Inst()->GetWindow()->GetWidth(),
										App::Inst()->GetWindow()->GetHeight());
	SetAspect(size);
	//SetViewport(0.0f, size.x, size.y, 0.0f);
}

glm::mat4 Camera::GetView() const
{
	if (_mMode == Mode::Perspective)
	{
		return glm::lookAt(GetPosition(), GetPosition() + GetForward(), _mUp);
	}
	else if (_mMode == Mode::Orthographic)
	{
		return glm::mat4(1.0f);
	}

	return glm::mat4(1.0f);
}

glm::mat4 Camera::GetProjection() const
{
	if (_mMode == Mode::Perspective)
	{
		return glm::perspective(_mFov, _mAspect, _mClip.x, _mClip.y);
	}
	else if (_mMode == Mode::Orthographic)
	{
		return glm::ortho(_mViewport[0], _mViewport[1], _mViewport[2], _mViewport[3], _mClip.x, _mClip.y);
	}

	return glm::mat4(1.0f);
}

void Camera::SetMode(Mode mode)
{
	_mMode = mode;
}

void Camera::SetAspect(float aspect)
{
	_mAspect = aspect;
}

void Camera::SetAspect(const glm::vec2& size)
{
	_mAspect = size.x / size.y;
}

void Camera::SetViewport(float left, float right, float bottom, float top)
{
	_mViewport = glm::vec4(left, right, bottom, top);
}

void Camera::SetViewport(const glm::vec4& viewport)
{
	_mViewport = viewport;
}

void Camera::SetClip(const glm::vec2& clip)
{
	_mClip = clip;
}

void Camera::SetForward(const glm::vec3& forward)
{
	SetRotation(glm::quatLookAt(glm::normalize(forward), _mUp));
}

glm::vec3 Camera::GetForward() const
{
	return glm::rotate(GetWorldRotation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

void Camera::SetLookAt(const glm::vec3& point)
{
	SetForward(point - GetPosition());
}

void Camera::HandleMovement(Direction dir, float dt)
{
	float movementSpeed = 0.25f;
	float velocity = movementSpeed * dt;

	glm::vec3 right = glm::normalize(glm::cross(_mUp, GetForward()));

	switch (dir)
	{
	case FORWARD:
		SetPosition(GetPosition() + (GetForward() * velocity));
		break;
	case BACKWARD:
		SetPosition(GetPosition() - (GetForward() * velocity));
		break;
	case LEFT:
		SetPosition(GetPosition() - (glm::normalize(glm::cross(GetForward(), _mUp)) * velocity));
		break;
	case RIGHT:
		SetPosition(GetPosition() + (glm::normalize(glm::cross(GetForward(), _mUp)) * velocity));
		break;
	case UP:
		SetPosition(GetPosition() + (glm::normalize(glm::cross(GetForward(), right)) * velocity));
		break;
	case DOWN:
		SetPosition(GetPosition() - (glm::normalize(glm::cross(GetForward(), right)) * velocity));
		break;
	default:
		break;
	}
}

void Camera::HandleRotation(float xoffset, float yoffset)
{
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_mYaw += xoffset;
	_mPitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_mPitch > 89.0f)
		_mPitch = 89.0f;
	if (_mPitch < -89.0f)
		_mPitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(_mPitch)) * cos(glm::radians(_mYaw));
	front.y = sin(glm::radians(_mPitch));
	front.z = cos(glm::radians(_mPitch)) * sin(glm::radians(_mYaw));
	SetForward(glm::normalize(front));

	glm::vec3 right = glm::normalize(glm::cross(GetForward(), _mUp));
	_mUp = glm::normalize(glm::cross(right, GetForward()));
}

/*void Camera::Init(glm::vec3 cameraPos, glm::vec3 cameraTarget)
{
    _mPosition = cameraPos;
    _mTarget   = cameraTarget;
	_mFoV = 45.0f;
	
	Window * window = App::Inst()->GetWindow();

    // 1 - FOV, 2 - Aspect Ratio, 3 - Near clipping, 4 - Far clipping
    _mProjectionMat =
        glm::perspective(glm::radians(_mFoV), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 10000.0f);

    // Camera Dir
	_mForward = glm::normalize(_mTarget - _mPosition);

	// Set world up
	_mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Camera Right Axis
    _mRight = glm::normalize(glm::cross(_mWorldUp, _mForward));

    // Camera Up Axis
    _mUp = glm::cross(_mForward, _mRight);

    // Look At
    _mViewMat = glm::lookAt(_mPosition, _mTarget, _mWorldUp);

    // View movement
    // glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f);
	_mVelocity = glm::vec3(0);
	_mAcceleration = glm::vec3(0);
	_mForce = glm::vec3(0);

	// Movement vals
	_mMovementSpeed = 0.1f;
	_mFirstMouse = true;
}

void Camera::Update(float dt)
{
	Window * window = App::Inst()->GetWindow();
	_mProjectionMat = glm::perspective(glm::radians(_mFoV), (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 10000.0f);
	_mViewMat = glm::lookAt(_mPosition, _mPosition + _mForward, _mUp);
}

void Camera::HandleMovement(Direction dir, float dt)
{
	float velocity = _mMovementSpeed * dt;

	switch (dir)
	{
	case FORWARD:
		_mPosition += _mForward * velocity;
		break;
	case BACKWARD:
		_mPosition -= _mForward * velocity;
		break;
	case LEFT:
		_mPosition -= glm::normalize(glm::cross(_mForward, _mUp)) * velocity;
		break;
	case RIGHT:
		_mPosition += glm::normalize(glm::cross(_mForward, _mUp)) * velocity;
		break;
	case UP:
		_mPosition += glm::normalize(glm::cross(_mForward, _mRight)) * velocity;
		break;
	case DOWN:
		_mPosition -= glm::normalize(glm::cross(_mForward, _mRight)) * velocity;
		break;
	default:
		break;
	}
}

void Camera::HandleRotation(float xoffset, float yoffset)
{
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_mYaw += xoffset;
	_mPitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_mPitch > 89.0f)
		_mPitch = 89.0f;
	if (_mPitch < -89.0f)
		_mPitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(_mPitch)) * cos(glm::radians(_mYaw));
	front.y = sin(glm::radians(_mPitch));
	front.z = cos(glm::radians(_mPitch)) * sin(glm::radians(_mYaw));
	_mForward = glm::normalize(front);
	_mRight = glm::normalize(glm::cross(_mForward, _mWorldUp));
	_mUp = glm::normalize(glm::cross(_mRight, _mForward));
}

void Camera::HandleFoV(float xoffset, float yoffset)
{
	if (_mFoV >= 1.0f && _mFoV <= 45.0f)
		_mFoV -= yoffset;
	if (_mFoV <= 1.0f)
		_mFoV = 1.0f;
	if (_mFoV >= 45.0f)
		_mFoV = 45.0f;
}

glm::vec2 Camera::GetResolution()
{
	Window * window = App::Inst()->GetWindow();
	return glm::vec2((float)window->GetWidth(), (float)window->GetHeight());
}
*/