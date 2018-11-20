#pragma once

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <d3dx9math.h>
#include <vector>

struct Particle {
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DCOLOR color;
	double life;

	Particle(D3DXVECTOR3 position, D3DXVECTOR3 velocity, D3DCOLOR color, float life) {
		this->position = position;
		this->velocity = velocity;
		this->color = color;
		this->life = life;
	}
};

struct ParticleVert {
	float x, y, z;
	D3DCOLOR color;
};

class System;
class Graphics;

class ParticleSystem {

protected:
	const int MaxParticles = 1000;
	std::vector<Particle* > ParticleContainer;
	IDirect3DVertexBuffer9* VertexBuffer_;
	const DWORD particleFvF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
public:
	ParticleSystem(System* system);
	void InitializeBuffer(Graphics* graphics);
	void ClearAllParticles();
	virtual void UpdateParticles(System* system) = 0;
	void Update(System* system);
	void Render(Graphics* graphics);
};

#endif