// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Camera.h"

#include "Asset/Shader.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/Graphics.h"
#include "Helper/Utility.h"
#include "Math/Math.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Asset/Mesh.h"
#include "Object/GameObject.h"
#include "Platform/Platform.h"
#include "Transform.h"

namespace Theodore {
  Camera::Camera() : Component("Camera"), nearClipPlane(0.1f), farClipPlane(5000.f), isOrthographic(false), renderTexture(nullptr) {
    ResetAspect();
    ResetFieldOfView();
		transform = this->gameObject->GetTransform();
  }

  Camera::Camera(const Vector3d& position) : Component("Camera"), nearClipPlane(0.1f), farClipPlane(5000.f), isOrthographic(false), renderTexture(nullptr) {
    ResetAspect();
    ResetFieldOfView();
		transform = this->gameObject->GetTransform();
    transform->SetPosition(position);
  }

  Camera::~Camera() { SafeDealloc(renderTexture); }

  void Camera::ResetAspect() const {
		pixelWidth = static_cast<float>(Platform::GetWidth());
		pixelHeight = static_cast<float>(Platform::GetHeight());
		aspect = pixelWidth / pixelHeight;
  }

  void Camera::ResetFieldOfView() const { fieldOfView = 60.0f; }

  void Camera::ResetProjectionMatrix() const { ResetAspect(); }

  Matrix4x4 Camera::GetProjectionMatrix() const {
    ResetProjectionMatrix();
    if (isOrthographic) {
			projectionMatrix = Matrix4x4::Orthogonal(-10.f, 10.f, -10.f, 10.f, nearClipPlane, farClipPlane);
    } else {
			projectionMatrix = Matrix4x4::Perspective(Math::Radians(fieldOfView), aspect, nearClipPlane, farClipPlane);
    }
    return projectionMatrix;
  }

  Matrix4x4 Camera::GetWorldToCameraMatrix() const {
		worldToCameraMatrix = Matrix4x4::LookAt(transform->GetLocalPosition(), transform->GetLocalPosition() + transform->GetForward(), transform->GetUp());
    return worldToCameraMatrix;
  }

  Matrix4x4 Camera::GetCameraToWorldMatrix() const {
		cameraToWorldMatrix = Matrix4x4::Inverse(GetWorldToCameraMatrix());
    return cameraToWorldMatrix;
  }

  Ray Camera::ScreenPointToRay(const Vector3d& position) {
    Ray ray;
    ray.origin = transform->GetLocalPosition();
    ray.direction = ScreenToWorldPoint(Vector2d(position.x, position.y));
    ray.inverseDirection = Vector3d::Inverse(ray.direction);
    return ray;
  }

  Vector3d Camera::ScreenToWorldPoint(const Vector2d& position) {
    int viewport[4];
    Graphics::GetViewport(viewport);
    Vector4d clipCoords;
    clipCoords.x = (2.f * (position.x - viewport[0])) / viewport[2] - 1.f;
    clipCoords.y = (2.f * (viewport[3] - position.y - (1 - viewport[1]))) / viewport[3] - 1.f;
    clipCoords.z = -1.f;  // forward
    clipCoords.w = 1.f;

    Vector4d eyeCoords = Matrix4x4::Inverse(GetProjectionMatrix()) * clipCoords;
    eyeCoords.z = -1.f;  // forward
    eyeCoords.w = 0.f;

    // not the GetCameraToWorldMatrix()
    Vector4d worldCoords = transform->GetLocalToWorldMatrix() * eyeCoords;

    return Vector3d(worldCoords.x, worldCoords.y, worldCoords.z).Normalize();
  }

  Vector2d Camera::WorldToScreenPoint(const Vector3d& position) { return Vector2d(); }

  void Camera::SetOrthographic(bool isOrtho) { isOrthographic = isOrtho; }

  float Camera::GetFieldOfView() const { return fieldOfView; }

  float Camera::GetNearClipPlane() const { return nearClipPlane; }

  float Camera::GetFarClipPlane() const { return farClipPlane; }

  float Camera::GetAspectRatio() const { return aspect; }

  void Camera::SetFieldOfView(float view) { fieldOfView = view; }

  void Camera::SetNearClipPlane(float near) { nearClipPlane = near; }

  void Camera::SetFarClipPlane(float far) { farClipPlane = far; }

  FrameBuffer* Camera::GetRenderTexture() const { return renderTexture; }

  void Camera::SetRenderTexture(FrameBuffer* texture) { renderTexture = texture; }

  // private functions, for consistency with other components.

  void Camera::Update(float deltaTime) {}

  void Camera::Render() {}

  bool Camera::CompareEquality(const Object& rhs) const { return false; }

  bool Camera::Destroy() { return false; }
}  // namespace Theodore
