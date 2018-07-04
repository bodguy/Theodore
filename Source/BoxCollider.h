#ifndef BoxCollider_h
#define BoxCollider_h

#include "Collider.h"
#include "Vector3d.h"

namespace Quark {
	class BoxCollider : public Collider {
	public:
		BoxCollider();
		virtual ~BoxCollider();

		virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) override;

	private:
		virtual void Update(double deltaTime) override;
		virtual void Render() override;
		virtual bool CompareEquality(const Object& rhs) const override;
		virtual bool Destroy() override;

	private:
		Vector3d mCenter;
		Vector3d mSize;
	};
}

#endif /* BoxCollider_h */