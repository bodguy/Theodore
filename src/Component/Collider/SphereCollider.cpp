// Copyright (C) 2017 by bodguy
// This code is licensed under Apache 2.0 license (see LICENSE.md for details)

#include "SphereCollider.h"
#include "Graphics/Graphics.h"
#include "Component/MeshRenderer.h"
#include "Component/Transform.h"
#include "Asset/Mesh.h"
#include "Core/GameObject.h"

namespace Theodore {
  SphereCollider::SphereCollider() : Collider("SphereCollider"), center(), radius(1.f), maxLengthVector() {
		colliderType = ColliderType::Sphere;
  }

  SphereCollider::~SphereCollider() {}

  bool SphereCollider::Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) {
    // TODO: Error!!!!!!!
    Vector3d L = center - ray.origin;
    float tca = Vector3d::DotProduct(L, ray.direction);
    float d2 = Vector3d::DotProduct(L, L) - tca * tca;
    if (d2 > radius) return false;

    float thc = std::sqrt(radius - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
      t0 = t1;                   // if t0 is negative, let's use t1 instead
      if (t0 < 0) return false;  // both t0 and t1 are negative
    }

    return true;
  }

  Vector3d SphereCollider::GetCenter() const { return center; }

  void SphereCollider::SetCenter(const Vector3d& center) { this->center = center; }

  float SphereCollider::GetRadius() const { return radius; }

  void SphereCollider::SetRadius(float radius) { this->radius = radius; }

  void SphereCollider::CalculateBoundingVolumes() {
    MeshRenderer* meshRenderer = gameObject->GetComponent<MeshRenderer>();
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
						maxLengthVector = (*iter);
          }

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

				radius = maxLength;
				center = (max + min) * 0.5f;
      }
    }
  }

  void SphereCollider::Update(float deltaTime) {
    Matrix4x4 world = transform->GetLocalToWorldMatrix();
    Matrix4x4 model = transform->GetWorldToLocalMatrix();
    Vector3d scale = Matrix4x4::DecomposeScale(world);

    float newRadius = (maxLengthVector * scale).Length();
    Vector3d newCenter = Matrix4x4::DecomposeTranslation(world) +
                         Mathf::Pow(scale, 2.f) * Vector3d(
                           Mathf::Dot(Vector3d(model.rows[0]), center),
                           Mathf::Dot(Vector3d(model.rows[1]), center),
                           Mathf::Dot(Vector3d(model.rows[2]), center)
                         );

    Graphics::DrawSphere(newCenter, newRadius, Color::orange);
  }

  void SphereCollider::Render() {}

  bool SphereCollider::CompareEquality(const Object& rhs) const { return false; }

  bool SphereCollider::Destroy() { return false; }
}  // namespace Theodore