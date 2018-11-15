#pragma once

#ifndef ENEMYSHIP_H_INCLUDED
#define ENEMYSHIP_H_INCLUDED

#include "System.h"
#include "ShipBase.h"
#include "Ship.h"

class Graphics;

class EnemyShip : public ShipBase
{
public:
	EnemyShip(System* system, ShipBase* targetShip);

	void Update(System *system);
	void rotateToTarget();
	void moveTowardTarget();
	void shoot();

protected:
	float const slowingRadius = 150;
	ShipBase* targetShip_;
};

#endif