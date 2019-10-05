#include "Collider.h"

namespace Theodore {
  Collider::Collider(const std::string& name) : Component(name), mType(ColliderType::Undefined) {}

  Collider::~Collider() {}

  ColliderType Collider::GetType() const { return mType; }
} // namespace Theodore