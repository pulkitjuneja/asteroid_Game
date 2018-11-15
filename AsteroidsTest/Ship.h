#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

#include "System.h"
#include "Font.h"
#include "Asteroid.h"
#include "ShipBase.h"

class Graphics;

class PlayerShip : public ShipBase
{
public:
	PlayerShip(System* system);

	void SetControlInput(float acceleration,
		float rotation);

	void Update(System *system);
	void Render(Graphics *graphics) const;
	int GetTotalScore();
	void ResetScore();
	void addScore(int score );
	void CalculateScoreFromAsteroidSize(Asteroid* asteroid);

protected:
	Keyboard* keyboard;
	int recentScore, totalScore;
	Font* floatingScore_;
	double floatScoreCounter; // timer to show the floating text for

};

#endif // SHIP_H_INCLUDED
