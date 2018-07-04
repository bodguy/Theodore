#ifndef Collider_h
#define Collider_h

#include "Component.h"
#include "Enumeration.h"
#include "Ray.h"
#include "Physics.h"
#include <string>

namespace Quark {
	class Collider : public Component {
	public:
		Collider(const std::string& name);
		virtual ~Collider();

		ColliderType GetType() const;
		virtual bool Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) = 0;

	protected:
		ColliderType mType;
	};
}

#endif /* Collider_h */