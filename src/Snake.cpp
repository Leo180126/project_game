#include "snake.h"

Snake::Snake(GameField* field, sf::Texture* textures,float speed)
	: field_(field),
	tileEdgeLength_((float)field_->tileEdgeLength()),
	speed_(speed),
	textures_(textures)
{
	timer_ = sf::Clock();

	// Head segment
	Segment* bufferSegm = new Segment(
		{ tileEdgeLength_ * 2, 0.f },
		tileEdgeLength_,
		Segment::Type::Head,
		Direction::Right);

	bufferSegm->setTexture(textures_)
		.setRotation(getRotation_(Direction::Right));

	segmentList_.push_back(bufferSegm);
	field_->setSnakeBlockAt((int)tileEdgeLength_ * 2, 0);

	// Body segment
	bufferSegm = new Segment(
		{ tileEdgeLength_, 0.f },
		tileEdgeLength_,
		Segment::Type::Body,
		Direction::Right);

	bufferSegm->setTexture(textures_)
		.setRotation(getRotation_(Direction::Right));

	segmentList_.push_back(bufferSegm);
	field_->setSnakeBlockAt((int)tileEdgeLength_, 0);

	// Tail segment
	bufferSegm = new Segment(
		{ 0.f, 0.f },
		tileEdgeLength_,
		Segment::Type::Tail,
		Direction::Right);
	
	bufferSegm->setTexture(textures_)
		.setRotation(getRotation_(Direction::Right));
	
	segmentList_.push_back(bufferSegm);
	field_->setSnakeBlockAt(0, 0);
}

Snake::~Snake()
{
	for (auto segmentPointer : segmentList_)
		delete segmentPointer;
}

void Snake::update(Direction::Type direction, sf::Vector2f foodPos)
{
	deltaTime_ += timer_.getElapsedTime().asSeconds();
	timer_.restart();

	isFoodEaten_ = false;

	if (deltaTime_ >= speed_)
	{
		deltaTime_ = 0;

		// Head movement
		Segment* head = segmentList_.front();
		if (direction != Direction::opposite(head->direction()) and
			direction != Direction::None)
		{
			head->setDirection(direction)
				.setRotation(getRotation_(direction));
		}

		Segment* bufferSegm = new Segment(*head);

		sf::Vector2f newPosOffset = getNewPositionOffset_(head->direction());
		sf::Vector2f newPos = head->position() + newPosOffset;

		if (!field_->isPosInField((int)newPos.x, (int)newPos.y) or
			isPosOnSnake_(newPos))
		{
			isDead_ = true;
			return;
		}

		head->setPosition(newPos);
		field_->setSnakeBlockAt((int)newPos.x, (int)newPos.y);

		// Segment after the head movement
		if (newPos == foodPos)
		{
			isFoodEaten_ = true;
		}
		else
		{
			sf::Vector2f pos = bufferSegm->position();
			delete bufferSegm;

			bufferSegm = new Segment(**std::prev(segmentList_.end(), 2));
			bufferSegm->setDirection(head->direction());
			bufferSegm->setPosition(pos);
		}

		segmentList_.insert(std::next(segmentList_.begin()), bufferSegm);

		if (head->direction() == (*std::next(segmentList_.begin(), 2))->direction())
		{
			bufferSegm->setRotation(getRotation_(head->direction()))
				.setType(Segment::Type::Body);

		}
		else
		{
			bufferSegm->setRotation(getRotation_(head->direction(), (*std::next(segmentList_.begin(), 2))->direction()))
				.setType(Segment::Type::Turn);
		}

		// Tail movement
		if (!isFoodEaten_)
		{
			bufferSegm = segmentList_.back();
			segmentList_.pop_back();

			sf::Vector2f pos = bufferSegm->position();
			field_->removeSnakeBlockAt((int)pos.x, (int)pos.y);

			Segment* segmentToDelete = segmentList_.back();
			pos = segmentToDelete->position();
			delete segmentToDelete;

			bufferSegm->setPosition(pos);

			Direction::Type newDirection = getDirectionFromPositions_(
				bufferSegm->position(),
				(*std::prev(segmentList_.end(), 2))->position());

			bufferSegm->setDirection(newDirection);
			bufferSegm->setRotation(getRotation_(bufferSegm->direction()));

			segmentList_.back() = bufferSegm;
		}
	}
}

sf::Vector2f Snake::getNewPositionOffset_(Direction::Type direction) const
{
	sf::Vector2f offset;
	switch (direction)
	{
	case Direction::Left:
		offset.x -= tileEdgeLength_;
		break;
	case Direction::Right:
		offset.x += tileEdgeLength_;
		break;
	case Direction::Up:
		offset.y -= tileEdgeLength_;
		break;
	case Direction::Down:
		offset.y += tileEdgeLength_;
		break;
	}
	return offset;
}

Direction::Type Snake::getDirectionFromPositions_(sf::Vector2f position, sf::Vector2f position2) const
{
	float dx = position2.x - position.x;
	float dy = position2.y - position.y;

	if (std::abs(dx) >= std::abs(dy)) {
		return (dx > 0) ? Direction::Right : Direction::Left;
	}
	else {
		return (dy > 0) ? Direction::Down : Direction::Up;
	}
}

float Snake::getRotation_(Direction::Type direction, Direction::Type direction2) const
{
	float rotation = 0;

	if (direction2 != Direction::None)
	{
		if (direction - 1 == direction2) rotation += 180;
		else if (direction + 1 == direction2) rotation += 90;
	}

	switch (direction)
	{
	case Direction::Left:
		rotation += 270;
		break;
	case Direction::Right:
		rotation += 90;
		break;
	case Direction::Up:
		rotation += 0;
		break;
	case Direction::Down:
		rotation += 180;
		break;
	}

	return ((int)rotation >= 360) ? rotation - 360 : rotation;
}

bool Snake::isPosOnSnake_(sf::Vector2f pos) const
{
	for (auto segm : segmentList_)
		if (pos == segm->position()) return true;
	return false;
}

const std::vector<const sf::Drawable*> Snake::getRectsToDraw() const
{
	std::vector<const sf::Drawable*> vect;
	for (auto segment : segmentList_)
		vect.push_back(segment->getRectToDraw());

	return vect;
}