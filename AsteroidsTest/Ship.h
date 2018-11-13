#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "GameEntity.h"
#include "System.h"
#include "Font.h"
#include "Asteroid.h"

class Graphics;

class Ship : public GameEntity
{
public:
	Ship(System* system);

	void SetControlInput(float acceleration,
		float rotation);

	void Update(System *system);
	void Render(Graphics *graphics) const;
	int GetTotalScore();
	void ResetScore();
	void addScore(Asteroid* asteroid);

	D3DXVECTOR3 GetForwardVector() const;
	D3DXVECTOR3 GetVelocity() const;
	void Reset();

private:

	float accelerationControl_;
	float rotationControl_;
	int recentScore, totalScore;
	D3DXVECTOR3 velocity_;
	D3DXVECTOR3 forward_;
	float rotation_;
	Font* floatingScore_;
	double floatScoreCounter; // timer to show the floating text for

};

#endif // SHIP_H_INCLUDED
