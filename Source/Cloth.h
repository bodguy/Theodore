/**
  @brief cloth simulation class
  @author bodguy
  @date 17.07.17
  @todo
  @bug
*/

#ifndef Cloth_h
#define Cloth_h

#include "Camera.h"
#include "Constraint.h"
#include "Particle.h"
#include "Shader.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include <vector>

namespace Theodore {
  class Particle;
  class Cloth {
  public:
    Cloth(int width, int height);
    ~Cloth();

    static float GetDamping();
    Particle* GetParticle(int x, int y);
    void AddConstraint(Particle* p1, Particle* p2);

    Vector3d calcTriangleNormal(Particle* p1, Particle* p2, Particle* p3);
    void addWindForcesForTriangle(Particle* p1, Particle* p2, Particle* p3,
                                  const Vector3d& direction);
    void WindForce(const Vector3d& direction);

    void Render(const Camera& cam);
    void Update(float dt);

  private:
    static float Damping;
    bool mEnabled;
    bool mUseGravity;
    float mClothSolverFrequency;
    int mWidth;
    int mHeight;
    std::vector<Particle> mParticles;
    std::vector<Constraint> mConstraint;
    Transform mTransform;
    VertexArray mVao;
    Buffer mBuffer;
    InputStream mStream;
    Pipeline mProgram;
  };
}

#endif // Cloth_h