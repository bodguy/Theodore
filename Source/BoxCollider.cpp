#include "BoxCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Graphics.h"

namespace Quark {
	BoxCollider::BoxCollider() : Collider("BoxCollider"), mSize(Vector3d(3.f, 5.f, 3.f)) {
		mType = ColliderType::Box;
	}

	BoxCollider::~BoxCollider() {
	
	}

	Vector3d BoxCollider::GetCenter() const {
		return mCenter;
	}
	
	Vector3d BoxCollider::GetSize() const {
		return mSize;
	}

	bool BoxCollider::Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) {
		Vector3d min = mGameObject->GetTransform()->GetWorldToLocalMatrix() * mCenter - mSize * 0.5f;
		Vector3d max = mGameObject->GetTransform()->GetWorldToLocalMatrix() * mCenter + mSize * 0.5f;

		float t1 = (min[0] - ray.origin[0]) * ray.invDirection[0];
		float t2 = (max[0] - ray.origin[0]) * ray.invDirection[0];

		float tmin = std::fminf(t1, t2);
		float tmax = std::fmaxf(t1, t2);

		for (unsigned int i = 1; i < 3; i++) {
			t1 = (min[i] - ray.origin[i]) * ray.invDirection[i];
			t2 = (max[i] - ray.origin[i]) * ray.invDirection[i];

			tmin = std::fmaxf(tmin, std::fminf(t1, t2));
			tmax = std::fminf(tmax, std::fmaxf(t1, t2));
		}

		return tmax > std::fmaxf(tmin, 0.f);
	}

	void BoxCollider::Update(double deltaTime) {

	}

	void BoxCollider::Render() {
		if (!mIsRender) {
			Graphics::DrawCube(mGameObject->GetTransform()->TransformPoint(mCenter), mSize, Color::green);
		}
	}

	bool BoxCollider::CompareEquality(const Object& rhs) const {
		return false;
	}

	bool BoxCollider::Destroy() {
		return false;
	}
}