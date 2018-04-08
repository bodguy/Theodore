#ifndef Particle_h
#define Particle_h

#include "Vector3d.h"

namespace Quark {
	class Particle {
	public:
		Particle();
		Particle(const Vector3d& vec);
		~Particle();

		Vector3d& GetPosition();
		Vector3d& GetNormal();

		void ResetAcceleration();
		void ResetNormal();

		void OffsetPos(const Vector3d& v);
		void SetMovable(bool b);
		void AddToNormal(Vector3d normal);
		void AddForce(const Vector3d& f);

		void Update(float dt);

	private:
		Vector3d mPosition;
		Vector3d mOldPosition;
		Vector3d mAcceleration;
		Vector3d mAccmulatedNormal;
		float mMass;
		bool mMovable;
	};
}

#endif // Particle_h