#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    sf::VideoMode videoMode(600, 600); // Window size
    std::string windowTitle = "Slithering Snake";
    auto windowStyles = sf::Style::Titlebar | sf::Style::Close;

    sf::RenderWindow window(videoMode, windowTitle, windowStyles);
    //Chuyển vị trí cửa sổ
    sf::Vector2i winPosit_ (0,0);
    window.setPosition(winPosit_);
    Game game(&window);
    game.run();

    return 0;
}