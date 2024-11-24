#ifndef SNAKE_GAME_SNAKE_DIRECTION_DIRECTION_H_
#define SNAKE_GAME_SNAKE_DIRECTION_DIRECTION_H_

#include <SFML/Window/Keyboard.hpp>
#include <opencv2/opencv.hpp>
class Direction
{
public:
	enum Type
	{
		None,
		Up,
		Right,
		Down,
		Left
	};

	// Convert SFML key to direction
	// static Direction::Type fromSfmlKey(sf::Keyboard::Key key);
	static Direction::Type fromFingertipPosition(cv::Point fingertip);
	// Get opposite direction
	static Direction::Type opposite(Direction::Type dir);

private:
	Direction() = delete;
};

// Shift the direction counterclockwise by the number that is being subtracted
Direction::Type operator-(Direction::Type dir, int num);

// Shift the direction counterclockwise by the number that is being subtracted
Direction::Type operator-(int num, Direction::Type dir);

// Shift the direction clockwise by the number that is being added
Direction::Type operator+(Direction::Type dir, int num);

// Shift the direction clockwise by the number that is being added
Direction::Type operator+(int num, Direction::Type dir);

#endif // SNAKE_GAME_SNAKE_DIRECTION_DIRECTION_H_