#include "FoodSpawner.h"

FoodSpawner::FoodSpawner(GameField* field, sf::Texture* texture)
	: field_(field)
{
	std::random_device rd;
	gen_ = new std::mt19937(rd());

	foodUnit_.setSize({ (float)field_->tileEdgeLength(), (float)field_->tileEdgeLength() });
	foodUnit_.setTexture(texture);

	respawn();
}

FoodSpawner::~FoodSpawner()
{
	delete gen_;
}

void FoodSpawner::respawn()
{
	auto rows = field_->getRowsWithFreeTiles();
	int yPos = getRand_((int)rows.size());
	yPos = field_->normalizeVerticalPos(yPos);
	
	int xPos = getRand_(field_->freeTilesCount(yPos));
	xPos = field_->normalizeHorizontalPos(yPos, xPos);

	foodUnit_.setPosition((float)xPos, (float)yPos);
}

int FoodSpawner::getRand_(int maxNum) const
{
	std::uniform_int_distribution<> d(0, maxNum - 1);
	return d(*gen_);
}