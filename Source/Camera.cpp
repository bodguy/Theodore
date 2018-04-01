#include "Camera.h"
#include "Math.h"
#include "Platform.h"
#include "os_types.h"
#include GLEW_INCLUDE_DIR

namespace Quark {
	Camera::Camera(const Vector3d& position) : mNearClipPlane(0.1f), mFarClipPlane(1000.f), mOrthographic(false) {
		mTransform.SetPosition(position);
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
		mProjectionMatrix = Matrix4x4::Perspective(Math::Radians(mFieldOfView), mAspect, mNearClipPlane, mFarClipPlane);
		return mProjectionMatrix;
	}

	Matrix4x4 Camera::GetWorldToCameraMatrix() const {
		mWorldToCameraMatrix = Matrix4x4::LookAt(mTransform.GetPosition(), mTransform.GetPosition() + mTransform.GetForward(), Vector3d::up);
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
}
