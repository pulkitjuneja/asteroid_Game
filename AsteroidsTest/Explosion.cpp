#include "Explosion.h"
#include "System.h"

Explosion::Explosion(System * system, D3DCOLOR color):
	ParticleSystem(system), explosionColor(color)
{
}

void Explosion::Emit(int numParticles, D3DXVECTOR3 position)
{
	if (ParticleContainer.size() < this->MaxParticles) {
		for (int i = 0; i < numParticles; i++) {
			float velX = 2 * ((float)rand() / (float)RAND_MAX) - 1;
			float velY = 2 * ((float)rand() / (float)RAND_MAX) - 1;
			Particle* particle = new Particle(position, D3DXVECTOR3(velX * 50, velY * 50, 0), explosionColor, explosionLife);
			ParticleContainer.push_back(particle);
		}
	}
}

void Explosion::UpdateParticles(System * system)
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
