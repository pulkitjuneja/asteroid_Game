#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED

#include "GameEntity.h"
#include "ParticleSystem.h"

class System;

class Explosion : public ParticleSystem
{
	D3DCOLOR explosionColor;
	const double explosionLife = 2.0;
public:
	Explosion(System* system, D3DCOLOR color);
	void Emit(int numParticles, D3DXVECTOR3 position);
	void UpdateParticles(System * system);
};

#endif // EXPLOSION_H_INCLUDED
