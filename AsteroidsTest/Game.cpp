#include "Game.h"
#include "OrthoCamera.h"
#include "Background.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Random.h"
#include "Maths.h"
#include "Bullet.h"
#include "Collision.h"
#include "EnemyShip.h"
#include <algorithm>

Game::Game(System* system) :
	camera_(0),
	background_(0),
	player_(0),
	collision_(0),
	systemRef_(system),
	enemyShip_(nullptr)
{
	camera_ = new OrthoCamera();
	camera_->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera_->SetFrustum(800.0f, 600.0f, -100.0f, 100.0f);
	background_ = new Background(800.0f, 600.0f);
	collision_ = new Collision();
	bulletManager = new BulletManager(collision_);
	scoreFont_ = system->GetGraphics()->CreateXFont("Arial", 20);
	collisionExplosion = new Explosion(systemRef_, 0xffff00);
}

Game::~Game()
{
	delete camera_;
	delete background_;
	delete player_;
	DeleteAllAsteroids();
	bulletManager->DeleteAllBullets();
	DeleteEnemy();
	DeleteAllExplosions();
	delete collision_;
}

void Game::Update(System *system)
{
	collisionExplosion->Update(system);
	UpdatePlayer(system);
	UpdateAsteroids(system);
	bulletManager->UpdateBullets(system);
	UpdateCollisions();
	updateEnemies(system);
}

void Game::RenderBackgroundOnly(Graphics *graphics)
{
	camera_->SetAsView(graphics);
	background_->Render(graphics);
}

void Game::RenderEverything(Graphics *graphics)
{
	camera_->SetAsView(graphics);

	background_->Render(graphics);

	if (player_)
	{
		player_->Render(graphics);
	}

	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		(*asteroidIt)->Render(graphics);
	}

	bulletManager->RenderBullets(graphics);

	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
		++explosionIt)
	{
		(*explosionIt)->Render(graphics);
	}

	// rendeing Player score
	// rendering it here as I believe that the game class should have control of where the score is rendered
	if (player_ != nullptr) {
		string scoreText = "Total Score : " + std::to_string(player_->GetTotalScore());
		int textX = (800 - scoreFont_->CalculateTextWidth(scoreText) - 20);
		scoreFont_->DrawText(scoreText, textX, 5, 0xffffff00);
	}

	if (enemyShip_ != nullptr) {
		enemyShip_->Render(graphics);
	}

	collisionExplosion->Render(graphics);
}

void Game::InitialiseLevel(int level)
{
	DeleteAllAsteroids();
	DeleteAllExplosions();
	DeleteEnemy();
	bulletManager->DeleteAllBullets();
	SpawnPlayer();
	SpawnEnemy(level);
	SpawnAsteroids(level);
}

bool Game::IsLevelComplete() const
{
	return (asteroids_.empty() && explosions_.empty());
}

bool Game::IsGameOver() const
{
	return (player_ == nullptr && explosions_.empty());
}

void Game::DoCollision(GameEntity *a, GameEntity *b)
{
	PlayerShip *player = static_cast<PlayerShip *>(a == player_ ? a : (b == player_ ? b : 0));
	Bullet *bullet = static_cast<Bullet *>(bulletManager->IsBullet(a) ? a : (bulletManager->IsBullet(b) ? b : 0));
	Asteroid *asteroid = static_cast<Asteroid *>(IsAsteroid(a) ? a : (IsAsteroid(b) ? b : 0));
	EnemyShip* enemy = static_cast<EnemyShip*>(a == enemyShip_ ? a : (b == enemyShip_ ? b : 0));

	if (player && asteroid)
	{
		AsteroidHit(asteroid);
		DeletePlayer();
	}

	if (bullet) {
		collisionExplosion->Emit(20, bullet->GetPosition());
		if (asteroid) {
			player_->CalculateScoreFromAsteroidSize(asteroid);
			AsteroidHit(asteroid);
			bulletManager->DeleteBullet(bullet);
		} else if (player && bullet->shipRef_ != player_) {
			bulletManager->DeleteBullet(bullet);
			DeletePlayer();
		}
		else if (enemy && bullet->shipRef_ == player_) {
			bulletManager->DeleteBullet(bullet);
			player_->addScore(100);
			DeleteEnemy();
		}

	}

}

void Game::SpawnPlayer()
{
	DeletePlayer();
	player_ = new PlayerShip(systemRef_);
	player_->EnableCollisions(collision_, 10.0f);
}

void Game::DeletePlayer()
{
	delete player_;
	player_ = nullptr;
}

void Game::DeleteEnemy()
{
	delete enemyShip_;
	enemyShip_ = nullptr;
}

void Game::SpawnEnemy(int level)
{
	if (level > 2) {
		const int spawnPositions[2] = { -200, 200 };
		int xRandomizer = rand() % 2;
		int yRandomizer = rand() % 2;
		enemyShip_ = new EnemyShip(systemRef_, player_);
		enemyShip_->EnableCollisions(collision_, 10.0);
		enemyShip_->SetPosition(D3DXVECTOR3(spawnPositions[xRandomizer], spawnPositions[yRandomizer], 0));
	}
}

void Game::UpdatePlayer(System *system)
{
	if (player_ != nullptr) {
		player_->Update(system);
		player_->WrapEntity();
	}
}

void Game::UpdateAsteroids(System *system)
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
	++asteroidIt)
	{
		(*asteroidIt)->Update(system);
		(*asteroidIt)->WrapEntity();
	}
}

void Game::updateEnemies(System * system)
{
	if (enemyShip_ != nullptr) {
		enemyShip_->Update(system);
	}
}

void Game::DeleteAllAsteroids()
{
	for (AsteroidList::const_iterator asteroidIt = asteroids_.begin(),
		end = asteroids_.end();
		asteroidIt != end;
		++asteroidIt)
	{
		delete (*asteroidIt);
	}

	asteroids_.clear();
}

void Game::DeleteAllExplosions()
{
	for (ExplosionList::const_iterator explosionIt = explosions_.begin(),
		end = explosions_.end();
		explosionIt != end;
	++explosionIt)
	{
		delete (*explosionIt);
	}

	explosions_.clear();
}

void Game::SpawnAsteroids(int numAsteroids)
{
	float halfWidth = 800.0f * 0.5f;
	float halfHeight = 600.0f * 0.5f;
	for (int i = 0; i < numAsteroids; i++)
	{
		float x = Random::GetFloat(-halfWidth, halfWidth);
		float y = Random::GetFloat(-halfHeight, halfHeight);
		D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0.0f);
		SpawnAsteroidAt(position, 3);
	}
}

void Game::SpawnAsteroidAt(const D3DXVECTOR3 &position, int size)
{
	const float MAX_ASTEROID_SPEED = 1.0f;

	float angle = Random::GetFloat(Maths::TWO_PI);
	D3DXMATRIX randomRotation;
	D3DXMatrixRotationZ(&randomRotation, angle);
	D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, Random::GetFloat(MAX_ASTEROID_SPEED), 0.0f);
	D3DXVec3TransformNormal(&velocity, &velocity, &randomRotation);

	Asteroid *asteroid = new Asteroid(position, velocity, size);
	asteroid->EnableCollisions(collision_, size * 5.0f);
	asteroids_.push_back(asteroid);
}

bool Game::IsAsteroid(GameEntity *entity) const
{
	return (std::find(asteroids_.begin(),
		asteroids_.end(), entity) != asteroids_.end()); 
}

void Game::AsteroidHit(Asteroid *asteroid)
{
	int oldSize = asteroid->GetSize();
	if (oldSize > 1)
	{
		int smallerSize = oldSize -1;
		D3DXVECTOR3 position = asteroid->GetPosition();
		SpawnAsteroidAt(position, smallerSize);
		SpawnAsteroidAt(position, smallerSize);
	}
	DeleteAsteroid(asteroid);
}

void Game::DeleteAsteroid(Asteroid *asteroid)
{
	asteroids_.remove(asteroid);
	delete asteroid;
}

void Game::UpdateCollisions()
{
	collision_->DoCollisions(this);
}
