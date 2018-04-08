#ifndef Cloth_h
#define Cloth_h

#include <vector>
#include "Particle.h"
#include "Constraint.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "Shader.h"

namespace Quark {
	class Particle;
	class Cloth {
	public:
		Cloth(int width, int height);
		~Cloth();

		static float GetDamping();
		Particle* GetParticle(int x, int y);
		void AddConstraint(Particle* p1, Particle* p2);

		Vector3d calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);
		void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const Vector3d& direction);
		void WindForce(const Vector3d& direction);

		void Render();
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
		Program mProgram;
	};
}

#endif // Cloth_h