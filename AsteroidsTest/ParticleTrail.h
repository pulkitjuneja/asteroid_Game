#pragma once 

#ifndef PARTICLE_TRAIL_H

#include "ParticleSystem.h"

class ParticleTrail : public ParticleSystem {

	D3DXVECTOR3 position, direction;
	const double trailLife = 0.8;
public:
	ParticleTrail(System* system);
	void SetPosition(D3DXVECTOR3 &position);
	void SetTrailDirection(D3DXVECTOR3 &direction);
	void Emit(int numParticles);
	D3DXVECTOR3 VectorInUnitSphere();
	void UpdateParticles(System* system);
};

#endif // !PARTICLE_TRAIL_H
