#include "Ship.h"
#include "Graphics.h"
#include "Maths.h"
#include "Keyboard.h"
#include <algorithm>
#include "ParticleTrail.h"

PlayerShip::PlayerShip(System* system) : 
	ShipBase(system),
	recentScore(0),
	totalScore(0)
{
	floatingScore_ = system->GetGraphics()->CreateXFont("Arial", 20);
	keyboard = system->GetKeyboard();
	MAX_SPEED = 2.0;
	trail = new ParticleTrail(system);
}

void PlayerShip::SetControlInput(float acceleration,
	float rotation)
{
	accelerationControl_ = acceleration;
	rotationControl_ = rotation;
}

void PlayerShip::Update(System *system)
{

	trail->SetPosition(GetPosition());
	trail->SetTrailDirection(GetForwardVector());
	trail->Update(system);

	accelerationControl_ = 0.0f;
	if (keyboard->IsKeyHeld(VK_UP) || keyboard->IsKeyHeld('W'))
	{
		trail->Emit(10);
		accelerationControl_ = 1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_DOWN) || keyboard->IsKeyHeld('S'))
	{
		accelerationControl_ = -1.0f;
	}

	rotationControl_ = 0.0f;
	if (keyboard->IsKeyHeld(VK_RIGHT) || keyboard->IsKeyHeld('D'))
	{
		rotationControl_ = -1.0f;
	}
	else if (keyboard->IsKeyHeld(VK_LEFT) || keyboard->IsKeyHeld('A'))
	{
		rotationControl_ = 1.0f;
	}

	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		std::chrono::duration<float> elapsed = Time::now() - lastBulletFireTime;
		if (elapsed.count() > FIRE_GAP) {
			D3DXVECTOR3 playerForward = GetForwardVector();
			D3DXVECTOR3 bulletPosition = GetPosition() + playerForward * 10.0f;
			bulletManagerRef_->SpawnBullet(bulletPosition, playerForward, this, this->shipColor);
			lastBulletFireTime = Time::now();
		}
	}

	if (floatScoreCounter > 0) {
		floatScoreCounter -= system->deltaTime;
	}

	const float RATE_OF_ROTATION = 0.1f;

	rotation_ = Maths::WrapModulo(rotation_ + rotationControl_ * RATE_OF_ROTATION,
		Maths::TWO_PI);
	ShipBase::Update(system);
}

void PlayerShip::Render(Graphics *graphics) const
{

	// Render recent floating text
	if (floatScoreCounter > 0) {
		string scoreString = "+" + std::to_string(recentScore);
		int textX = 800 - floatingScore_->CalculateTextWidth(scoreString) - 20; // padding 
		floatingScore_->DrawText(scoreString, textX, 25, 0xffffff00);
	}
	trail->Render(graphics);
	ShipBase::Render(graphics);
}

int PlayerShip::GetTotalScore()
{
	return totalScore;
}

void PlayerShip::ResetScore()
{
	totalScore = 0;
	recentScore = 0;
}

void PlayerShip::addScore(int score)
{
	totalScore += score;
	recentScore = score;
	floatScoreCounter = 2.0;
}

void PlayerShip::CalculateScoreFromAsteroidSize(Asteroid * asteroid)
{
	switch (asteroid->GetSize()) {
	case 1: addScore(50); break;
	case 2: addScore(20); break;
	case 3: addScore(10); break;
	}
}
