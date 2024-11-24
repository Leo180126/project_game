#include "Direction.h"

#include <cstdlib>
#include <cmath>


cv::Point snakeHeadPosition(300, 300); // Replace with actual snake head position as needed
constexpr int movementThreshold = 60;  // Threshold for directional changes

Direction::Type Direction::fromFingertipPosition(cv::Point fingertip)
{
    int dx = fingertip.x - snakeHeadPosition.x;
    int dy = fingertip.y - snakeHeadPosition.y;

    // Calculate the angle of movement
    float angle = std::atan2(dy, dx) * 180 / CV_PI;

    if (std::abs(dx) > movementThreshold || std::abs(dy) > movementThreshold) {
        if (angle >= -45 && angle <= 45) {
            return Direction::Right;
        } else if (angle > 45 && angle < 135) {
            return Direction::Down;
        } else if (angle >= 135 || angle <= -135) {
            return Direction::Left;
        } else if (angle > -135 && angle < -45) {
            return Direction::Up;
        }
    }
    return Direction::None;
}

Direction::Type Direction::opposite(Direction::Type dir)
{
	switch (dir)
	{
	case Direction::Left:
		return Direction::Right;
	case Direction::Right:
		return Direction::Left;
	case Direction::Up:
		return Direction::Down;
	case Direction::Down:
		return Direction::Up;
	}
	return Direction::None;
}

Direction::Type operator-(Direction::Type dir, int num)
{ return operator-(num, dir); }

Direction::Type operator-(int num, Direction::Type dir)
{
	if (dir and num != 0)
	{
		if (num < 0) return operator+(std::abs(num), dir);
		return (Direction::Type)(5 - (int)operator+(num, (Direction::Type)(5 - (int)dir)));
	}
	return dir;
}

Direction::Type operator+(Direction::Type dir, int num)
{ return operator+(num, dir); }

Direction::Type operator+(int num, Direction::Type dir)
{
	if (dir and num != 0)
	{
		if (num < 0) return operator-(std::abs(num), dir);

		int res = num - num / 4 * 4 + (int)dir;
		if (res > 4) return (Direction::Type)(res % 4);
		else return (Direction::Type)res;
	}
	return dir;
}