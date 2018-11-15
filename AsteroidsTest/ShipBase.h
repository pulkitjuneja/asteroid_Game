#pragma once

#ifndef SHIP_BASE_H
#define SHIP_BASE_H

#include "GameEntity.h"
#include "System.h"
#include "Graphics.h"
#include "BulletManager.h"

class ShipBase : public GameEntity {
public:
	ShipBase(System* system);
	virtual void Update(System *system);
	virtual void Render(Graphics *graphics) const;

	D3DXVECTOR3 GetForwardVector() const;
	D3DXVECTOR3 GetVelocity() const;
	virtual void Reset();

protected:
	BulletManager* bulletManagerRef_;
	float MAX_SPEED;
	float accelerationControl_;
	float rotationControl_;
	D3DXVECTOR3 velocity_;
	D3DCOLOR shipColor;
	D3DXVECTOR3 forward_;
	float rotation_;
	time_point lastBulletFireTime;
	double FIRE_GAP = 1.0;  // time between subsequent bullet fires
};

#endif // !SHIP_BASE_H

