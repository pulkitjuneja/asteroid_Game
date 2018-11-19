#include "ParticleTrail.h"
#include <stdlib.h>
#include "System.h"

ParticleTrail::ParticleTrail(System * system):
	ParticleSystem(system)
{
}

void ParticleTrail::SetPosition(D3DXVECTOR3 &position)
{
	this->position = position;
}

void ParticleTrail::SetTrailDirection(D3DXVECTOR3 & direction)
{
	this->direction = direction;
}

void ParticleTrail::Emit(int numParticles)
{
	if (ParticleContainer.size() < this->MaxParticles) {
		for (int i = 0; i < numParticles; i++) {
			D3DXVECTOR3 velocity = direction + VectorInUnitSphere();
			D3DXVec3Scale(&velocity, &velocity, 10);
			int randR = rand() % 256;
			int randG = rand() % 256;
			int randB = rand() % 256;
			D3DXCOLOR particleColor = D3DCOLOR_XRGB(randR, randG, randB);
			Particle* particle = new Particle(position, velocity, particleColor, trailLife);
			ParticleContainer.push_back(particle);
		}
	}
}

D3DXVECTOR3 ParticleTrail::VectorInUnitSphere()
{
	D3DXVECTOR3 unit;
	do
	{
		float randX = 2 * ((float)rand() / (float)RAND_MAX) - 1;
		float randY = 2 * ((float)rand() / (float)RAND_MAX) - 1;
		unit = D3DXVECTOR3(randX, randY, 0);
	} while (D3DXVec3LengthSq(&unit) > 1.0f);

	return unit;
}

void ParticleTrail::UpdateParticles(System * system)
{
	std::vector<Particle *>::iterator particleIter = ParticleContainer.begin();
	while (particleIter != ParticleContainer.end())
	{
		Particle* currentParticle = *particleIter;

		currentParticle->life -= system->deltaTime;

		if (currentParticle->life <= 0) {
			particleIter = ParticleContainer.erase(particleIter);
			continue;
		}

		D3DXVECTOR3 scaledVelocity;
		D3DXVec3Scale(&scaledVelocity, &currentParticle->velocity, system->deltaTime);
		currentParticle->position += scaledVelocity;

		particleIter++;
	}
}
