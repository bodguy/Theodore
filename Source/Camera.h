#ifndef Camera_h
#define Camera_h

#include "Vector3d.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "Ray.h"

namespace Quark {
	class Camera {
	public:
		Camera();
		Camera(const Vector3d& position);

		void ResetAspect() const;
		void ResetFieldOfView() const;
		void ResetProjectionMatrix() const;

		Matrix4x4 GetProjectionMatrix() const;
		Matrix4x4 GetWorldToCameraMatrix() const;
		Matrix4x4 GetCameraToWorldMatrix() const;

		Ray ScreenPointToRay(const Vector3d& position);
		Vector3d ScreenToWorldPoint(const Vector3d& position);
		void SetOrthographic(bool isOrtho);
		Transform& GetTransform();
	
	private:
		mutable float mFieldOfView;
		float mNearClipPlane;
		float mFarClipPlane;
		mutable float mPixelWidth;
		mutable float mPixelHeight;
		mutable float mAspect;
		Transform mTransform;

		mutable Matrix4x4 mProjectionMatrix;
		mutable Matrix4x4 mWorldToCameraMatrix;
		mutable Matrix4x4 mCameraToWorldMatrix;
		bool mOrthographic; // true->orthographic, false->perspective
	};
}

#endif /* Camera_h */