#include "ParticleSystem.h"
#include "Graphics.h"
#include "System.h"

ParticleSystem::ParticleSystem(System* system)
{
	ParticleContainer.reserve(MaxParticles);
	InitializeBuffer(system->GetGraphics());
}

void ParticleSystem::InitializeBuffer(Graphics* graphics)
{
	D3DPOOL pool = D3DPOOL_DEFAULT;
	DWORD usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
	HRESULT result = graphics->GetDevice()->CreateVertexBuffer(MaxParticles * sizeof(ParticleVert), usage, particleFvF, pool, &VertexBuffer_, NULL);
	if (FAILED(result)) {
		OutputDebugString("Failed to create vertex Buffer");
	}
}

void ParticleSystem::Update(System * system)
{
	UpdateParticles(system);

	ParticleVert* bufferData = new ParticleVert[ParticleContainer.size()];
	for (int i = 0; i < ParticleContainer.size(); i++) {
		ZeroMemory(&bufferData[i], sizeof(ParticleVert));
		bufferData[i].x = ParticleContainer[i]->position.x;
		bufferData[i].y = ParticleContainer[i]->position.y;
		bufferData[i].z = ParticleContainer[i]->position.z;
		bufferData[i].color = ParticleContainer[i]->color;
	}

	void *pData;
	if (FAILED(VertexBuffer_->Lock(0, 0, (void**)&pData, D3DLOCK_DISCARD))) {
		OutputDebugString("failed to lock vetex buffer\n");
	}
	memcpy(pData, bufferData, ParticleContainer.size() * sizeof(ParticleVert));
	VertexBuffer_->Unlock();

}

void ParticleSystem::Render(Graphics * graphics)
{
	graphics->DisableLighting();
	graphics->SetPointSize(2.0);
	graphics->SetVertexFormat(particleFvF);
	graphics->GetDevice()->SetStreamSource(0, VertexBuffer_, 0, sizeof(ParticleVert));
	graphics->GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, ParticleContainer.size());
	graphics->EnableLighting();
}
