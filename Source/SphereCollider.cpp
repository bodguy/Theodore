#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Graphics.h"
#include "MeshRenderer.h"
#include "Mesh.h"

namespace Theodore {
	SphereCollider::SphereCollider() : Collider("SphereCollider"), mCenter(), mMaxLengthVector() {
		mType = ColliderType::Sphere;
		CalculateBoundingVolumes();
	}

	SphereCollider::~SphereCollider() {
	
	}

	bool SphereCollider::Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) {
		// Error!!!!!!!
		Vector3d L = mCenter - ray.origin;
		float tca = Vector3d::DotProduct(L, ray.direction);
		float d2 = Vector3d::DotProduct(L, L) - tca * tca;
		if (d2 > mRadius) 
			return false;

		float thc = std::sqrt(mRadius - d2);
		float t0 = tca - thc;
		float t1 = tca + thc;

		if (t0 > t1) 
			std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) return false; // both t0 and t1 are negative 
		}

		return true;
	}

	Vector3d SphereCollider::GetCenter() const {
		return mCenter;
	}

	void SphereCollider::SetCenter(const Vector3d& center) {
		mCenter = center;
	}

	float SphereCollider::GetRadius() const {
		return mRadius;
	}

	void SphereCollider::SetRadius(float radius) {
		mRadius = radius;
	}

	void SphereCollider::CalculateBoundingVolumes() {
		MeshRenderer* meshRenderer = mGameObject->GetComponent<MeshRenderer>();
		if (meshRenderer) {
			Mesh* mesh = meshRenderer->GetMesh();

			if (mesh) {
				std::vector<Vector3d>::const_iterator iter;
				float maxLength = 0.f;

				for (iter = mesh->GetVertexData().cbegin(); iter < mesh->GetVertexData().cend(); iter++) {
					float length = (*iter).Length();
					if (length > maxLength) {
						maxLength = length;
						mMaxLengthVector = (*iter);
					}
				}

				SetRadius(maxLength);
			}
		}
		mCenter = mTransform->GetPosition();
	}

	void SphereCollider::Update(double deltaTime) {
		mRadius = (mMaxLengthVector * mTransform->GetLocalScale()).Length();
		mCenter = mTransform->GetPosition();
	}

	void SphereCollider::Render() {
		if (mIsVisible) {
			Graphics::DrawSphere(mCenter, mRadius, mColor);
		}
	}

	bool SphereCollider::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool SphereCollider::Destroy() {
		return false;
	}
}