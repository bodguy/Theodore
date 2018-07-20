/**
	@brief camera component contains all relevant camera functionality 
	@author bodguy
	@date 17.07.17
	@todo 
	@bug 
*/

#ifndef Camera_h
#define Camera_h

#include "Vector3d.h"
#include "Matrix4x4.h"
#include "Ray.h"
#include "Component.h"

namespace Theodore {
	class Transform; class FrameBuffer;
	class Camera : public Component {
	public:
		Camera();
		Camera(const Vector3d& position);
		virtual ~Camera();

		void ResetAspect() const;
		void ResetFieldOfView() const;
		void ResetProjectionMatrix() const;

		Matrix4x4 GetProjectionMatrix() const;
		Matrix4x4 GetWorldToCameraMatrix() const;
		Matrix4x4 GetCameraToWorldMatrix() const;

		Ray ScreenPointToRay(const Vector3d& position);
		Vector3d ScreenToWorldPoint(const Vector2d& position);
		Vector2d WorldToScreenPoint(const Vector3d& position);
		void SetOrthographic(bool isOrtho);

		float GetFieldOfView() const;
		float GetNearClipPlane() const;
		float GetFarClipPlane() const;
		float GetAspectRatio() const;
		void SetFieldOfView(float view);
		void SetNearClipPlane(float near);
		void SetFarClipPlane(float far);
		Transform* GetTransform() const;
		FrameBuffer* GetRenderTexture() const;
		void SetRenderTexture(FrameBuffer* texture);
		void PrepareFrustum();

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		mutable float mFieldOfView;
		float mNearClipPlane;
		float mFarClipPlane;
		mutable float mPixelWidth;
		mutable float mPixelHeight;
		mutable float mAspect;
		Transform* mTransform;
		FrameBuffer* mRenderTexture;

		mutable Matrix4x4 mProjectionMatrix;
		mutable Matrix4x4 mWorldToCameraMatrix;
		mutable Matrix4x4 mCameraToWorldMatrix;
		bool mOrthographic; // true->orthographic, false->perspective
	};
}

#endif /* Camera_h */