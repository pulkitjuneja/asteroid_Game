#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "GameEntity.h"

class ShipBase;

class Bullet : public GameEntity
{
public:
	Bullet(const D3DXVECTOR3 &position,
		const D3DXVECTOR3 &direction, ShipBase* shipref, D3DCOLOR color);

	double lifeTime;
	void Update(System *system);
	void Render(Graphics *graphics) const;
	ShipBase* shipRef_;

private:
	D3DCOLOR bulletColor;
	const double BULLET_LIFETIME = 5.0;
	D3DXVECTOR3 velocity_;

};

#endif // BULLET_H_INCLUDED
