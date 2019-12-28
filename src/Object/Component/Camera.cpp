// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Camera.h"

#include "Asset/Shader.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/Graphics.h"
#include "Helper/Utility.h"
#include "Math/Math.h"
#include "MeshRenderer.h"
#include "Object/Component/sub/Material.h"
#include "Object/Component/sub/Mesh.h"
#include "Object/GameObject.h"
#include "Platform/Platform.h"
#include "Transform.h"

namespace Theodore {
  Camera::Camera() : Component("Camera"), mNearClipPlane(0.1f), mFarClipPlane(5000.f), mOrthographic(false), mRenderTexture(nullptr) {
    ResetAspect();
    ResetFieldOfView();
    mTransform = this->mGameObject->GetTransform();
  }

  Camera::Camera(const Vector3d& position) : Component("Camera"), mNearClipPlane(0.1f), mFarClipPlane(5000.f), mOrthographic(false), mRenderTexture(nullptr) {
    ResetAspect();
    ResetFieldOfView();
    mTransform = this->mGameObject->GetTransform();
    mTransform->SetPosition(position);
  }

  Camera::~Camera() { SafeDealloc(mRenderTexture); }

  void Camera::ResetAspect() const {
    mPixelWidth = static_cast<float>(Platform::GetWidth());
    mPixelHeight = static_cast<float>(Platform::GetHeight());
    mAspect = mPixelWidth / mPixelHeight;
  }

  void Camera::ResetFieldOfView() const { mFieldOfView = 60.0f; }

  void Camera::ResetProjectionMatrix() const { ResetAspect(); }

  Matrix4x4 Camera::GetProjectionMatrix() const {
    ResetProjectionMatrix();
    if (mOrthographic) {
      mProjectionMatrix = Matrix4x4::Orthogonal(-10.f, 10.f, -10.f, 10.f, mNearClipPlane, mFarClipPlane);
    } else {
      mProjectionMatrix = Matrix4x4::Perspective(Math::Radians(mFieldOfView), mAspect, mNearClipPlane, mFarClipPlane);
    }
    return mProjectionMatrix;
  }

  Matrix4x4 Camera::GetWorldToCameraMatrix() const {
    mWorldToCameraMatrix = Matrix4x4::LookAt(mTransform->GetLocalPosition(), mTransform->GetLocalPosition() + mTransform->GetForward(), mTransform->GetUp());
    return mWorldToCameraMatrix;
  }

  Matrix4x4 Camera::GetCameraToWorldMatrix() const {
    mCameraToWorldMatrix = Matrix4x4::Inverse(GetWorldToCameraMatrix());
    return mCameraToWorldMatrix;
  }

  Ray Camera::ScreenPointToRay(const Vector3d& position) {
    Ray ray;
    ray.origin = mTransform->GetLocalPosition();
    ray.direction = ScreenToWorldPoint(Vector2d(position.x, position.y));
    ray.invDirection = Vector3d::Inverse(ray.direction);
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
    Vector4d worldCoords = mTransform->GetLocalToWorldMatrix() * eyeCoords;

    return Vector3d(worldCoords.x, worldCoords.y, worldCoords.z).Normalize();
  }

  Vector2d Camera::WorldToScreenPoint(const Vector3d& position) { return Vector2d(); }

  void Camera::SetOrthographic(bool isOrtho) { mOrthographic = isOrtho; }

  float Camera::GetFieldOfView() const { return mFieldOfView; }

  float Camera::GetNearClipPlane() const { return mNearClipPlane; }

  float Camera::GetFarClipPlane() const { return mFarClipPlane; }

  float Camera::GetAspectRatio() const { return mAspect; }

  void Camera::SetFieldOfView(float view) { mFieldOfView = view; }

  void Camera::SetNearClipPlane(float near) { mNearClipPlane = near; }

  void Camera::SetFarClipPlane(float far) { mFarClipPlane = far; }

  FrameBuffer* Camera::GetRenderTexture() const { return mRenderTexture; }

  void Camera::SetRenderTexture(FrameBuffer* texture) { mRenderTexture = texture; }

  // private functions, for consistency with other components.

  void Camera::Update(float deltaTime) {}

  void Camera::Render() {}

  bool Camera::CompareEquality(const Object& rhs) const { return false; }

  bool Camera::Destroy() { return false; }
}  // namespace Theodore
