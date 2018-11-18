#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <d3dx9math.h>
#include <list>
#include "BulletManager.h"
#include "Font.h"
#include "System.h"
#include "Graphics.h"

class OrthoCamera;
class Background;
class PlayerShip;
class EnemyShip;
class Bullet;
class Asteroid;
class Explosion;
class Collision;
class System;
class Graphics;
class GameEntity;


class Game
{
public:
	Game(System* system);
	~Game();

	void Update(System *system);
	void RenderBackgroundOnly(Graphics *graphics);
	void RenderEverything(Graphics *graphics);
	void InitialiseLevel(int numAsteroids);
	bool IsLevelComplete() const;
	bool IsGameOver() const;

	void DoCollision(GameEntity *a, GameEntity *b);

	BulletManager* bulletManager;

private:
	Game(const Game &);
	void operator=(const Game &);

	typedef std::list<Asteroid *> AsteroidList;
	typedef std::list<Explosion *> ExplosionList;

	void SpawnPlayer();
	void DeletePlayer();
	void DeleteEnemy();
	void SpawnEnemy(int level);

	void UpdatePlayer(System *system);
	void UpdateAsteroids(System *system);
	void updateEnemies(System* system);

	void DeleteAllAsteroids();
	void DeleteAllExplosions();

	void SpawnAsteroids(int numAsteroids);
	void SpawnAsteroidAt(const D3DXVECTOR3 &position, int size);
	bool IsAsteroid(GameEntity *entity) const;
	void AsteroidHit(Asteroid *asteroid);
	void DeleteAsteroid(Asteroid *asteroid);

	void UpdateCollisions();

	OrthoCamera *camera_;
	System* systemRef_;
	Background *background_;
	PlayerShip *player_;
	AsteroidList asteroids_;
	ExplosionList explosions_;
	Font* scoreFont_;
	EnemyShip* enemyShip_;
	Collision *collision_;
	Explosion* collisionExplosion;
};

#endif // GAME_H_INCLUDED
