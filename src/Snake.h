#ifndef SNAKE_GAME_SNAKE_SNAKE_H_
#define SNAKE_GAME_SNAKE_SNAKE_H_

#include <list>

#include <SFML/Graphics.hpp>

#include "Direction.h"
#include "Segment.h"
#include "GameField.h"

class Snake
{
public:
	Snake(GameField* field, sf::Texture* textures,float speed =0.1f);
	~Snake();

	void update(Direction::Type direction, sf::Vector2f foodPos);
	bool isFoodEaten() const { return isFoodEaten_; }
	bool isDead() const { return isDead_; }

	const std::vector<const sf::Drawable*> getRectsToDraw() const;

private:
	sf::Vector2f getNewPositionOffset_(Direction::Type direction) const;
	Direction::Type getDirectionFromPositions_(sf::Vector2f position, sf::Vector2f position2) const;

	// direction2 (direction of previous segment) should be set only if rotation is needed for turn segment.
	float getRotation_(Direction::Type direction, Direction::Type direction2 = Direction::None) const;
	
	bool isPosOnSnake_(sf::Vector2f pos) const;

	GameField* field_;
	float tileEdgeLength_;

	sf::Clock timer_;
	float deltaTime_ = 0;
	float speed_;

	sf::Texture* textures_;
	
	std::list<Segment*> segmentList_;

	bool isFoodEaten_{ false };
	bool isDead_ { false };
};

#endif // SNAKE_GAME_SNAKE_SNAKE_H_