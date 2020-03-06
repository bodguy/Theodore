// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "Object.h"

#include "Component/Transform.h"
#include "GameObject.h"
#include "Helper/crc32.h"
#include "Math/Quaternion.h"
#include "Math/Vector3d.h"

namespace Theodore {
  std::atomic<uint32_t> UniqueId::type_id;
  Object::Object(const std::string& name) : name(name), hashCode(CRC32_STR(name.c_str())) {}

  Object::Object(const Object& rhs) {
    // TODO
  }

  Object::~Object() {}

  bool Object::operator==(const Object& rhs) const { return hashCode == rhs.hashCode && this->CompareEquality(rhs); }

  bool Object::operator!=(const Object& rhs) const { return !(*this == rhs && this->CompareEquality(rhs)); }

  const std::string& Object::ToString() const { return name; }

  const uint32_t Object::GetHashCode() const { return hashCode; }

  uint32_t Object::GetInstanceID() const { return instanceId; }

  void Object::SetName(const std::string& name) {
		this->name = name;
		hashCode = CRC32_STR(name.c_str());
  }

  bool Object::Destroy(GameObject* obj) {
    if (!obj) return false;

    return obj->Destroy();
  }

  GameObject* Object::Instantiate(GameObject* original) {
    if (!original) return nullptr;

    return new GameObject(*original);
  }

  GameObject* Object::Instantiate(GameObject* original, const Vector3d& position) {
    if (!original) return nullptr;

    GameObject* clone = new GameObject(*original);
    clone->GetTransform()->SetPosition(position);

    return clone;
  }

  GameObject* Object::Instantiate(GameObject* original, const Vector3d& position, const Quaternion& rotation) {
    if (!original) return nullptr;

    GameObject* clone = new GameObject(*original);
    clone->GetTransform()->SetPosition(position);
    clone->GetTransform()->SetRotation(rotation);

    return clone;
  }
}  // namespace Theodore