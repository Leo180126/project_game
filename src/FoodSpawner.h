#ifndef SNAKE_GAME_FOOD_FOOD_SPAWNER_H
#define SNAKE_GAME_FOOD_FOOD_SPAWNER_H

#include <random>

#include <SFML/Graphics.hpp>

#include "GameField.h"

class FoodSpawner
{
public:
	FoodSpawner(GameField* field, sf::Texture* texture);
	~FoodSpawner();

	sf::Vector2f currentPos() const { return foodUnit_.getPosition(); }

	sf::Drawable* getRectToDraw() const { return (sf::Drawable*) &foodUnit_; }

	void respawn();

private:
	int getRand_(int maxNum) const;

	std::mt19937* gen_;

	GameField* field_;

	sf::RectangleShape foodUnit_;
};

#endif // SNAKE_GAME_FOOD_FOOD_SPAWNER_H