#include "ShipBase.h"
#include "Game.h"
#include <iostream>

ShipBase::ShipBase(System * system):
accelerationControl_(0.0f),
rotationControl_(0.0f),
velocity_(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
forward_(D3DXVECTOR3(0.0f, 1.0f, 0.0f)),
shipColor(0xffffff),
rotation_(0.0f) //**TODO: Candidate for crash
{
	bulletManagerRef_ = system->GetGame()->bulletManager;
}

void ShipBase::Update(System * system)
{

	const float VELOCITY_TWEEN = 0.05f;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationZ(&rotationMatrix, rotation_);
	D3DXVec3TransformNormal(&forward_, &D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		&rotationMatrix);
	D3DXVec3Normalize(&forward_, &forward_);

	D3DXVECTOR3 idealVelocity;
	D3DXVec3Scale(&idealVelocity, &forward_, accelerationControl_ * MAX_SPEED);
	D3DXVec3Lerp(&velocity_, &velocity_, &idealVelocity, VELOCITY_TWEEN);

	D3DXVECTOR3 newPosition = GetPosition();
	D3DXVec3Add(&newPosition, &newPosition, &velocity_);
	SetPosition(newPosition);
}

void ShipBase::Render(Graphics * graphics) const
{
	struct DummyVert
	{
		float x, y, z;
		D3DCOLOR diffuse;
	};

	DummyVert axis[8] =
	{
		{0.0f, -5.0f, 0.0f, shipColor}, {0.0f, 10.0f, 0.0f, shipColor},
		{-5.0f, 0.0f, 0.0f, shipColor}, {5.0f, 0.0f, 0.0f, shipColor},
		{0.0f, 10.0f, 0.0f, shipColor}, {-5.0f, 5.0f, 0.0f, shipColor},
		{0.0f, 10.0f, 0.0f, shipColor}, {5.0f, 5.0f, 0.0f, shipColor},
	};

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationZ(&rotationMatrix, rotation_);

	D3DXVECTOR3 position = GetPosition();
	D3DXMATRIX translationMatrix;
	D3DXMatrixTranslation(&translationMatrix,
		position.x,
		position.y,
		position.z);

	D3DXMATRIX shipTransform = rotationMatrix * translationMatrix;

	D3DXMATRIX identityMatrix;
	D3DXMatrixIdentity(&identityMatrix);

	DWORD dummyFvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	graphics->SetVertexFormat(dummyFvf);
	graphics->DisableLighting();
	graphics->SetModelMatrix(&shipTransform);
	graphics->DrawImmediate(D3DPT_LINELIST,
		4,
		&axis[0],
		sizeof(axis[0]));

	graphics->SetModelMatrix(&identityMatrix);
	graphics->EnableLighting();
}

D3DXVECTOR3 ShipBase::GetForwardVector() const
{
	return forward_;
}

D3DXVECTOR3 ShipBase::GetVelocity() const
{
	return velocity_;
}

void ShipBase::Reset()
{
	accelerationControl_ = 0.0f;
	rotationControl_ = 0.0f;

	velocity_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	forward_ = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	rotation_ = 0.0f;

	SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}
