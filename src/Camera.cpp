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
	float velocity = _mMovementSpeed * dt;

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
	glm::vec2 delta(xoffset, yoffset);

	delta *= _mRotateSpeed;
	delta.x *= -1.0f;

	if (_mInverse)
		delta.y *= -1.0f;

	glm::vec3 forward = GetForward();
	glm::vec3 right = glm::normalize(glm::cross(GetWorldUp(), forward));
	SetUp(glm::cross(forward, right));

	glm::quat rotation = GetRotation();
	rotation = glm::angleAxis(delta.x, GetUp()) * rotation;
	rotation = glm::angleAxis(delta.y, GetRight()) * rotation;
	SetRotation(rotation);
}