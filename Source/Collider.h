#ifndef Collider_h
#define Collider_h

#include "Component.h"
#include "Enumeration.h"
#include "Ray.h"
#include "Physics.h"
#include "Color.h"
#include <string>

namespace Quark {
	class Collider : public Component {
	public:
		Collider(const std::string& name);
		virtual ~Collider();

		ColliderType GetType() const;
		void SetVisible(bool isVisible);
		void SetColor(const Color& color);
		virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) = 0;

	private:
		virtual void CalculateBoundingVolumes() = 0;

	protected:
		ColliderType mType;
		bool mIsVisible;
		Color mColor;
	};
}

#endif /* Collider_h */