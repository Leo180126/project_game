#ifndef SNAKE_GAME_GAMEFIELD_H
#define SNAKE_GAME_GAMEFIELD_H

#include <vector>

#include <SFML/Graphics.hpp>

namespace gf_
{
	struct Position { int x, y; };
}

class GameField
{
public:
	GameField(int width, int height, int tileEdgeLength, sf::Texture* texture);
	~GameField();

	int width() { return width_; }
	int height() { return height_; }
	int tileEdgeLength() { return tileEdgeLength_; }
	int tileSum() { return tileSum_; }

	void setSnakeBlockAt(int x, int y);
	void removeSnakeBlockAt(int x, int y);

	std::vector<std::vector<bool>> getRowsWithFreeTiles() const;
	int freeTilesCount(int rowIndex) const;
	int normalizeVerticalPos(int y) const;
	int normalizeHorizontalPos(int y, int x) const;

	bool isPosInField(int x, int y) const;

	sf::Drawable* getRectToDraw() const { return (sf::Drawable*) &fieldRect_; };

private:
	gf_::Position normalizePosition_(int x, int y) const;

	sf::RectangleShape fieldRect_;
	int width_, height_, tileEdgeLength_, tileSum_;
	std::vector<std::vector<bool>> matrix_;
	std::vector<int> matrixCapacity_;
};

#endif // SNAKE_GAME_GAMEFIELD_H