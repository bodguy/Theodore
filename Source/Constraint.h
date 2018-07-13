#ifndef Constraint_h
#define Constraint_h

namespace Theodore {
	class Particle;
	class Constraint {
	public:
		Constraint(Particle* p1, Particle* p2);
		~Constraint();

		void Satisfy();

	private:
		float mRestDistance;
		Particle* mP1;
		Particle* mP2;
	};
}

#endif // Constraint_h
