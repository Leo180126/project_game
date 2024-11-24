#ifndef SNAKE_GAME_GAME_H
#define SNAKE_GAME_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "GameField.h"
#include "Snake.h"
#include "FoodSpawner.h"
#include "Menu.h"

class Game
{
public:
    Game(sf::RenderWindow *window);
    ~Game();

    void run();

private:
    sf::RenderWindow* window_;
    std::array<std::unique_ptr<sf::Texture>, 3> textures_; // 0 - field, 1 - food, 2 - snake

    std::unique_ptr<GameField> field_;
    float tileEdgeLength_;
    bool isPaused_;
    std::unique_ptr<Snake> snake_;
    std::unique_ptr<FoodSpawner> food_;
    int highScore_;
    Menu menu_;
    bool isMenuActive_;
    int difficultyLevel_; // 0 - Easy, 1 - Medium, 2 - Hard
};

#endif // SNAKE_GAME_GAME_H