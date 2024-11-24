#ifndef SNAKE_GAME_SNAKE_SEGMENT_SEGMENT_H_
#define SNAKE_GAME_SNAKE_SEGMENT_SEGMENT_H_

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Direction.h"

class Segment
{
public:
	enum class Type { Head, Body, Tail, Turn };

	Segment(sf::Vector2f position, float size, Segment::Type type, Direction::Type direction = Direction::None);
	Segment(const Segment& segment);

	const Segment::Type& type() const { return type_; }
	Segment& setType(Segment::Type newType);

	const Direction::Type& direction() const { return direction_; }
	Segment& setDirection(Direction::Type direction);

	sf::Vector2f position() const;
	Segment& setPosition(const sf::Vector2f& pos);

	const float rotation() const { return rect_.getRotation(); }
	Segment& setRotation(float rotation);

	Segment& setTexture(sf::Texture* texture);

	const sf::Drawable* getRectToDraw() const { return &rect_; }

private:
	void updateTextureRect_();

	float originOffset_;
	int textureSegmentWidth_;

	Type type_;
	sf::RectangleShape rect_;
	Direction::Type direction_;
};

#endif // SNAKE_GAME_SNAKE_SEGMENT_SEGMENT_H_