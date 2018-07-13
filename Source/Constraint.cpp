#include "Constraint.h"
#include "Particle.h"

namespace Theodore {
	Constraint::Constraint(Particle* p1, Particle* p2) :mP1(p1), mP2(p2) {
		Vector3d vec = p1->GetPosition() - p2->GetPosition();
		mRestDistance = vec.Length();
	}

	Constraint::~Constraint() {
	}

	void Constraint::Satisfy() {
		Vector3d p1_to_p2 = mP2->GetPosition() - mP1->GetPosition();
		float current_distance = p1_to_p2.Length();
		Vector3d correctionVector = p1_to_p2 * (1.f - mRestDistance / current_distance);
		Vector3d correctionVectorHelf = correctionVector * 0.5f;
		mP1->OffsetPos(correctionVectorHelf);
		mP2->OffsetPos(-correctionVectorHelf);
	}
}