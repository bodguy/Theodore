#include "Camera.h"
#include "Math.h"
#include "Platform.h"
#include "os_types.h"
#include GLEW_INCLUDE_DIR

namespace Quark {
	Camera::Camera() : mNearClipPlane(0.1f), mFarClipPlane(5000.f), mOrthographic(false), mTransform() {
		mTransform.SetPosition(Vector3d(-0.101061f, 0.632650f, 2.383647f));
		ResetAspect();
		ResetFieldOfView();
	}

	Camera::Camera(const Vector3d& position) : mNearClipPlane(0.1f), mFarClipPlane(5000.f), mOrthographic(false) {
		mTransform.SetPosition(position);
		ResetAspect();
		ResetFieldOfView();
	}

	void Camera::ResetAspect() const {
		mPixelWidth = static_cast<float>(Platform::GetWidth());
		mPixelHeight = static_cast<float>(Platform::GetHeight());
		mAspect = mPixelWidth / mPixelHeight;
	}

	void Camera::ResetFieldOfView() const {
		mFieldOfView = 90.0f;
	}

	void Camera::ResetProjectionMatrix() const {
		ResetAspect();
		ResetFieldOfView();
	}

	Matrix4x4 Camera::GetProjectionMatrix() const {
		ResetProjectionMatrix();
		if (mOrthographic) {
			mProjectionMatrix = Matrix4x4::Orthogonal(-2.f, 2.f, -2.f, 2.f, -mNearClipPlane, mFarClipPlane);
		} else {
			mProjectionMatrix = Matrix4x4::Perspective(Math::Radians(mFieldOfView), mAspect, mNearClipPlane, mFarClipPlane);
		}
		return mProjectionMatrix;
	}

	Matrix4x4 Camera::GetWorldToCameraMatrix() const {
		mWorldToCameraMatrix = Matrix4x4::LookAt(mTransform.GetPosition(), mTransform.GetPosition() + mTransform.GetForward(), mTransform.GetUp());
		return mWorldToCameraMatrix;
	}
	
	Matrix4x4 Camera::GetCameraToWorldMatrix() const {
		mCameraToWorldMatrix = Matrix4x4::Inverse(GetWorldToCameraMatrix());
		return mCameraToWorldMatrix;
	}

	Ray Camera::ScreenPointToRay(const Vector3d& position) {
		Ray ray;
		ray.mOrigin = ScreenToWorldPoint(Vector3d(position.x, position.y, 0.f));
		ray.mDirection = ScreenToWorldPoint(Vector3d(position.x, position.y, 1.f));
		ray.mDirection = (ray.mDirection - ray.mOrigin).Normalize();
		return ray;
	}

	Vector3d Camera::ScreenToWorldPoint(const Vector3d& position) {
		Matrix4x4 invProjectionView = Matrix4x4::Inverse(GetProjectionMatrix()) * GetCameraToWorldMatrix();
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		Vector3d screenCoords(position.x - viewport[0], viewport[3] - position.y - 1 - viewport[1], position.z);

		screenCoords.x = (2 * screenCoords.x) / viewport[2] - 1.f;
		screenCoords.y = (2 * screenCoords.y) / viewport[3] - 1.f;
		screenCoords.z = 2 * screenCoords.z - 1;

		return invProjectionView * screenCoords;
	}

	Vector3d Camera::WorldToScreenPoint(const Vector3d& position) {
		return Vector3d();
	}

	void Camera::SetOrthographic(bool isOrtho) {
		mOrthographic = isOrtho;
	}

	Transform& Camera::GetTransform() {
		return mTransform;
	}

	float Camera::GetNearClipPlane() const {
		return mNearClipPlane;
	}

	float Camera::GetFarClipPlane() const {
		return mFarClipPlane;
	}

	void Camera::SetNearClipPlane(float near) {
		mNearClipPlane = near;
	}
		
	void Camera::SetFarClipPlane(float far) {
		mFarClipPlane = far;
	}
}
