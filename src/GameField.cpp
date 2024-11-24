#include <iostream>

#include "GameField.h"

using namespace gf_;

GameField::GameField(int width, int height, int tileEdgeLength, sf::Texture* texture)
	: width_(width), height_(height), tileEdgeLength_(tileEdgeLength)
{
	fieldRect_.setSize({ (float)width_ * tileEdgeLength_, (float)height_ * tileEdgeLength_ });

	texture->setRepeated(true);

	fieldRect_.setTexture(texture);
	fieldRect_.setTextureRect(sf::IntRect(0, 0,
		width_ * tileEdgeLength_,
		height_ * tileEdgeLength_));

	matrix_.resize(height_);
	for (int i = 0; i < height_; ++i)
	{
		matrix_[i].resize(width_);
		for (int j = 0; j < width_; ++j)
			matrix_[i][j] = false;
	}

	matrixCapacity_.resize(height_);
	for (int i = 0; i < height_; ++i)
		matrixCapacity_[i] = 0;

	tileSum_ = (width_ / tileEdgeLength_) * (height_ / tileEdgeLength_);
}

GameField::~GameField() {}

Position GameField::normalizePosition_(int x, int y) const
{
	if (!(x % tileEdgeLength_) and !(y % tileEdgeLength_))
		return Position{ x / tileEdgeLength_, y / tileEdgeLength_ };
	return Position{ 0, 0 };
}

void GameField::setSnakeBlockAt(int x, int y)
{
	Position pos = normalizePosition_(x, y);
	matrix_[pos.y][pos.x] = true;
	++matrixCapacity_[pos.y];
}

void GameField::removeSnakeBlockAt(int x, int y)
{
	Position pos = normalizePosition_(x, y);
	matrix_[pos.y][pos.x] = false;
	--matrixCapacity_[pos.y];
}

std::vector<std::vector<bool>> GameField::getRowsWithFreeTiles() const
{
	std::vector<std::vector<bool>> vect;
	for (int i = 0; i < height_; ++i)
		if (matrixCapacity_[i] < width_) vect.push_back(matrix_[i]);

	return vect;
}

int GameField::normalizeVerticalPos(int y) const
{
	for (int i = 0, j = y; i < height_; ++i)
	{
		if (matrixCapacity_[i] < width_)
		{
			if (j) --j;
			else return i * tileEdgeLength_;
		}
	}
	return -1;
}

int GameField::freeTilesCount(int y) const
{
	return width_ - matrixCapacity_[y / tileEdgeLength_];
}

int GameField::normalizeHorizontalPos(int y, int x) const
{
	int rowIndex = y / tileEdgeLength_;

	for (int i = 0, j = x; i < width_; ++i)
	{
		if (!matrix_[rowIndex][i])
		{
			if (j) --j;
			else return i * tileEdgeLength_;
		}
	}
	return -1;
}

bool GameField::isPosInField(int x, int y) const
{
	if ((x >= 0 and x < width_ * tileEdgeLength_) and (y >= 0 and y < height_ * tileEdgeLength_))
		return true;
	return false;
}