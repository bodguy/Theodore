// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "BoxCollider.h"
#include "Graphics/Graphics.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "Asset/Mesh.h"
#include "Core/GameObject.h"
#include "Math/Mathf.h"

namespace Theodore {
  BoxCollider::BoxCollider() : Collider("BoxCollider"), center(), size() {
		colliderType = ColliderType::Box;
  }

  BoxCollider::~BoxCollider() {}

  Vector3d BoxCollider::GetCenter() const { return center; }

  void BoxCollider::SetCenter(const Vector3d& center) { this->center = center; }

  Vector3d BoxCollider::GetSize() const { return size; }

  void BoxCollider::SetSize(const Vector3d& size) { this->size = size; }

  bool BoxCollider::Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) {
    Vector3d min = center - size * 0.5f;
    Vector3d max = center + size * 0.5f;

    float t1 = (min[0] - ray.origin[0]) * ray.inverseDirection[0];
    float t2 = (max[0] - ray.origin[0]) * ray.inverseDirection[0];

    float tmin = Mathf::Min(t1, t2);
    float tmax = Mathf::Max(t1, t2);

    for (unsigned int i = 1; i < 3; i++) {
      t1 = (min[i] - ray.origin[i]) * ray.inverseDirection[i];
      t2 = (max[i] - ray.origin[i]) * ray.inverseDirection[i];

      tmin = Mathf::Max(tmin, Mathf::Min(t1, t2));
      tmax = Mathf::Min(tmax, Mathf::Max(t1, t2));
    }

    return tmax > Mathf::Max(tmin, 0.f);
  }

  void BoxCollider::CalculateBoundingVolumes() {
    MeshRenderer* meshRenderer = gameObject->GetComponent<MeshRenderer>();
    if (meshRenderer) {
      Mesh* mesh = meshRenderer->GetMesh();

      if (mesh) {
        std::vector<Vector3d>::const_iterator iter;
        Vector3d min, max;

        for (iter = mesh->GetVertexData().cbegin(); iter < mesh->GetVertexData().cend(); iter++) {
          if ((*iter).x < min.x) {
            min.x = (*iter).x;
          } else if ((*iter).x > max.x) {
            max.x = (*iter).x;
          }

          if ((*iter).y < min.y) {
            min.y = (*iter).y;
          } else if ((*iter).y > max.y) {
            max.y = (*iter).y;
          }

          if ((*iter).z < min.z) {
            min.z = (*iter).z;
          } else if ((*iter).z > max.z) {
            max.z = (*iter).z;
          }
        }

				center = (max + min) * 0.5f;
				size = (center - min) * 2.f;
      }
    }
  }

  void BoxCollider::Update(float deltaTime) {}

  void BoxCollider::Render() { Graphics::DrawCube(center, size, Color::purple, transform->GetLocalToWorldMatrix()); }

  bool BoxCollider::CompareEquality(const Object& rhs) const { return false; }

  bool BoxCollider::Destroy() { return false; }
}  // namespace Theodore