#include "BulletManager.h"

bool BulletManager::IsBullet(GameEntity * entity) const
{
	return (std::find(bullets.begin(),
		bullets.end(), entity) != bullets.end());
}

void BulletManager::SpawnBullet(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction, Collision* collisionSystem)
{
	std::chrono::duration<float> elapsed = Time::now() - lastBulletFireTime;
	if (elapsed.count() > FIRE_GAP) {
		Bullet* newBullet = new Bullet(position, direction);
		newBullet->EnableCollisions(collisionSystem, 3.0f);
		bullets.push_back(newBullet);
		lastBulletFireTime = Time::now();
	}
}

void BulletManager::DeleteBullet(Bullet * bullet)
{
	bullets.remove(bullet);
	delete bullet;
}

void BulletManager::DeleteAllBullets()
{
	for (std::list<Bullet*>::iterator bulletIter = bullets.begin(); bulletIter != bullets.end(); bulletIter++) {
		delete (*bulletIter);
	}
	bullets.clear();
}

void BulletManager::UpdateBullets(System * system)
{
	std::list<Bullet*>::iterator bulletIter = bullets.begin();
	while (bulletIter != bullets.end()) {
		(*bulletIter)->lifeTime -= system->deltaTime;
		if ((*bulletIter)->lifeTime <= 0) {
			DeleteBullet((*bulletIter++)); // increments first and then passes, ensures iterator stays valid
		}
		else {
			(*bulletIter)->Update(system);
			(*bulletIter)->WrapEntity();
			bulletIter++;
		}
	}
}

void BulletManager::RenderBullets(Graphics * graphics)
{
	for (std::list<Bullet*>::const_iterator bulletIter = bullets.begin(); bulletIter != bullets.end(); bulletIter++) {
		(*bulletIter)->Render(graphics);
	}
}
