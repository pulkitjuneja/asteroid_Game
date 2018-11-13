#pragma once

#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <list>
#include "Bullet.h"
#include "System.h"

class BulletManager {
	std::list<Bullet*> bullets;
	time_point lastBulletFireTime;
	const double FIRE_GAP = 1.0;  // time between subsequent bullet fires
public:
	bool IsBullet(GameEntity* entity) const;
	BulletManager() {};
	void SpawnBullet(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction, Collision* collisionSystem);
	void DeleteBullet(Bullet* bullet);
	void DeleteAllBullets();
	void UpdateBullets(System* system);
	void RenderBullets(Graphics* graphics);
};

#endif // !BULLET_MANAGER_H
