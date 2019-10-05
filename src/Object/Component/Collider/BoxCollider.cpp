#include "BoxCollider.h"
#include "../../../Graphics/Graphics.h"
#include "../../GameObject.h"
#include "../MeshRenderer.h"
#include "../Transform.h"
#include "../sub/Mesh.h"

namespace Theodore {
  BoxCollider::BoxCollider() : Collider("BoxCollider"), mCenter(), mSize() {
    mType = ColliderType::Box;
    CalculateBoundingVolumes();
  }

  BoxCollider::~BoxCollider() {}

  Vector3d BoxCollider::GetCenter() const { return mCenter; }

  void BoxCollider::SetCenter(const Vector3d& center) { mCenter = center; }

  Vector3d BoxCollider::GetSize() const { return mSize; }

  void BoxCollider::SetSize(const Vector3d& size) { mSize = size; }

  bool BoxCollider::Raycast(const Ray& ray, RaycastHit& hitInfo, float maxDistance) {
    Vector3d min = mCenter - mSize * 0.5f;
    Vector3d max = mCenter + mSize * 0.5f;

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

  void BoxCollider::CalculateBoundingVolumes() {
    MeshRenderer* meshRenderer = mGameObject->GetComponent<MeshRenderer>();
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

        mCenter = (max + min) * 0.5f;
        mSize = (mCenter - min) * 2.f;
      }
    }
  }

  void BoxCollider::Update(float deltaTime) {}

  void BoxCollider::Render() { Graphics::DrawCube(mCenter, mSize, Color::purple, mTransform->GetLocalToWorldMatrix()); }

  bool BoxCollider::CompareEquality(const Object& rhs) const { return false; }

  bool BoxCollider::Destroy() { return false; }
}  // namespace Theodore