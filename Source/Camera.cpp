#include "Camera.h"
#include "Math.h"
#include "Platform.h"
#include "GameObject.h"
#include "Transform.h"
#include "Graphics.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Shader.h"
#include "Utility.h"
#include "FrameBuffer.h"

namespace Quark {
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

	Camera::~Camera() {
		SafeDealloc(mRenderTexture);
	}

	void Camera::ResetAspect() const {
		mPixelWidth = static_cast<float>(Platform::GetWidth());
		mPixelHeight = static_cast<float>(Platform::GetHeight());
		mAspect = mPixelWidth / mPixelHeight;
	}

	void Camera::ResetFieldOfView() const {
		mFieldOfView = 60.0f;
	}

	void Camera::ResetProjectionMatrix() const {
		ResetAspect();
		ResetFieldOfView();
	}

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
		mWorldToCameraMatrix = Matrix4x4::LookAt(mTransform->GetPosition(), mTransform->GetPosition() + mTransform->GetForward(), mTransform->GetUp());
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
		Graphics::GetViewport(viewport);
		Vector3d screenCoords(position.x - viewport[0], viewport[3] - position.y - (1 - viewport[1]), position.z);

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

	float Camera::GetFieldOfView() const {
		return mFieldOfView;
	}

	float Camera::GetNearClipPlane() const {
		return mNearClipPlane;
	}

	float Camera::GetFarClipPlane() const {
		return mFarClipPlane;
	}

	float Camera::GetAspectRatio() const {
		return mAspect;
	}

	void Camera::SetNearClipPlane(float near) {
		mNearClipPlane = near;
	}
		
	void Camera::SetFarClipPlane(float far) {
		mFarClipPlane = far;
	}

	Transform* Camera::GetTransform() const {
		return mTransform;
	}

	FrameBuffer* Camera::GetRenderTexture() const {
		return mRenderTexture;
	}

	void Camera::SetRenderTexture(FrameBuffer* texture) {
		mRenderTexture = texture;
	}

	void Camera::PrepareFrustum() {
		float tanAngle = tan(mFieldOfView / 2);

		float yNear = mNearClipPlane * tanAngle;
		float xNear = mAspect * yNear;
		float yFar = mFarClipPlane * tanAngle;
		float xFar = mAspect * yFar;

		Vector3d nearLeftTop(-xNear, yNear, -1.0f * mNearClipPlane);
		Vector3d nearLeftBottom(-xNear, -yNear, -1.0f * mNearClipPlane);
		Vector3d nearRightBottom(xNear, -yNear, -1.0f * mNearClipPlane);
		Vector3d nearRightTop(xNear, yNear, -1.0f * mNearClipPlane);

		Vector3d farLeftTop(-xFar, yFar, -1.0f * mFarClipPlane);
		Vector3d farLeftBottom(-xFar, -yFar, -1.0f * mFarClipPlane);
		Vector3d farRightBottom(xFar, -yFar, -1.0f * mFarClipPlane);
		Vector3d farRightTop(xFar, yFar, -1.0f * mFarClipPlane);

		std::vector<Vector3d> vertices = {
			nearLeftTop, nearLeftBottom, nearRightBottom, nearRightTop,     // front face
			farLeftTop, farLeftBottom, farRightBottom, farRightTop,         // far face
			nearLeftBottom, nearRightBottom, farRightBottom, farLeftBottom, // bottom face
			nearLeftTop, nearRightTop, farRightTop, farLeftTop,             // top face
			nearLeftBottom, farLeftBottom, farLeftTop, nearLeftTop,         // left face
			nearRightBottom, farRightBottom, farRightTop, nearRightTop      // right face
		};

		//MeshRenderer* rend = this->mGameObject->AddComponent<MeshRenderer>();

		//Mesh* mesh = new Mesh();
		//mesh->SetVertices(vertices);
		//mesh->MarkDynamic();
		//Material* mat = new Material(Shader::Find("Standard"));

		//rend->SetMaterial(mat);
		//rend->SetMesh(mesh);
	}

	// private functions, for consistency with other components.

	void Camera::Update(double deltaTime) {

	}

	void Camera::Render() {
		return;
	}

	bool Camera::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool Camera::Destroy() {
		return false;
	}
}
