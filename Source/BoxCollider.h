#ifndef BoxCollider_h
#define BoxCollider_h

#include "Collider.h"
#include "Vector3d.h"

namespace Theodore {
	class BoxCollider : public Collider {
	public:
		BoxCollider();
		virtual ~BoxCollider();

		Vector3d GetCenter() const;
		void SetCenter(const Vector3d& center);
		Vector3d GetSize() const;
		void SetSize(const Vector3d& size);

		virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) override;

	private:
		virtual void CalculateBoundingVolumes() override;

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