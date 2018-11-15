#pragma once

#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <list>
#include "Bullet.h"
#include "System.h"

class Game;

class BulletManager {
	std::list<Bullet*> bullets;
	Collision* collisionRef_;
public:
	bool IsBullet(GameEntity* entity) const;
	BulletManager(Collision* collisionref);
	void SpawnBullet(const D3DXVECTOR3 &position, const D3DXVECTOR3 &direction,
		ShipBase* shipRef, D3DCOLOR bulletColor);
	void DeleteBullet(Bullet* bullet);
	void DeleteAllBullets();
	void UpdateBullets(System* system);
	void RenderBullets(Graphics* graphics);
};

#endif // !BULLET_MANAGER_H
