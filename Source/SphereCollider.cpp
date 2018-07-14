#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Graphics.h"
#include "MeshRenderer.h"
#include "Mesh.h"

namespace Theodore {
	SphereCollider::SphereCollider() : Collider("SphereCollider"), mCenter(), mRadius(1.f), mMaxLengthVector() {
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
				Vector3d min, max;
				float maxLength = 0.f;

				for (iter = mesh->GetVertexData().cbegin(); iter < mesh->GetVertexData().cend(); iter++) {
					float length = (*iter).Length();
					if (length > maxLength) {
						maxLength = length;
						mMaxLengthVector = (*iter);
					}

					if ((*iter).x < min.x) {
						min.x = (*iter).x;
					}
					else if ((*iter).x > max.x) {
						max.x = (*iter).x;
					}

					if ((*iter).y < min.y) {
						min.y = (*iter).y;
					}
					else if ((*iter).y > max.y) {
						max.y = (*iter).y;
					}

					if ((*iter).z < min.z) {
						min.z = (*iter).z;
					}
					else if ((*iter).z > max.z) {
						max.z = (*iter).z;
					}
				}

				mRadius = maxLength;
				mCenter = (max + min) * 0.5f;
			}
		}
	}

	void SphereCollider::Update(double deltaTime) {

	}

	void SphereCollider::Render() {
		if (mIsVisible) {
			Matrix4x4 model = mTransform->GetLocalToWorldMatrix();
			float newRadius = (mMaxLengthVector * Matrix4x4::DecomposeScale(model)).Length();
			Vector3d newCenter = mCenter + Matrix4x4::DecomposeTranslation(model);

			Graphics::DrawSphere(newCenter, newRadius, mColor);
		}
	}

	bool SphereCollider::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool SphereCollider::Destroy() {
		return false;
	}
}