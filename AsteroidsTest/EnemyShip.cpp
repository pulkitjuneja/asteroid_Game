#include "EnemyShip.h"
#include <math.h>

EnemyShip::EnemyShip(System * system, ShipBase* targetShip): 
	ShipBase(system),
	targetShip_(targetShip)
{
	shipColor = 0xff0000;
	MAX_SPEED = 1.0f;
	FIRE_GAP = 2.0;
}

void EnemyShip::Update(System * system)
{
	rotateToTarget();
	moveTowardTarget();
	shoot();
	ShipBase::Update(system);
}

void EnemyShip::rotateToTarget()
{
	D3DXVECTOR3 scaledTargetForwardVec= targetShip_->GetForwardVector();
	float targetSpeed = D3DXVec3Length(&targetShip_->GetVelocity());
	D3DXVec3Scale(&scaledTargetForwardVec, &scaledTargetForwardVec,targetSpeed*50); // to shoot a little ahead of the player
	D3DXVECTOR3 target = targetShip_->GetPosition() + scaledTargetForwardVec;
	D3DXVECTOR3 forward = D3DXVECTOR3(0.0, 1.0, 0.0);
	D3DXVECTOR3 toPlayer =target - GetPosition();
	D3DXVec3Normalize(&toPlayer, &toPlayer);
	float rotation = (float)acos(D3DXVec3Dot(&forward, &toPlayer));
	if (toPlayer.x > 0) {
		rotation_ = Maths::TWO_PI - rotation;
	}
	else {
		rotation_ = rotation;
	}
}

void EnemyShip::moveTowardTarget()
{
	D3DXVECTOR3 toPlayer = targetShip_->GetPosition() - GetPosition();
	float distance = D3DXVec3Length(&toPlayer);
	if (distance < slowingRadius) {
		accelerationControl_ = 0;
	}
	else {
		accelerationControl_ = 1.0;
	}
}

void EnemyShip::shoot()
{
	std::chrono::duration<float> elapsed = Time::now() - lastBulletFireTime;
	if (elapsed.count() > FIRE_GAP) {
		int fireProbability = rand() % 10;
		if (fireProbability > 5) {
			D3DXVECTOR3 playerForward = GetForwardVector();
			D3DXVECTOR3 bulletPosition = GetPosition() + playerForward * 10.0f;
			bulletManagerRef_->SpawnBullet(bulletPosition, playerForward, this, this->shipColor);
		}
		lastBulletFireTime = Time::now();
	}
}
