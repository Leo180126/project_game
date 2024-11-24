#include "Menu.h"

Menu::Menu(float width, float height) : selectedItemIndex_(0), state_(MainMenu) {
    if (!font_.loadFromFile("C:\\Users\\84333\\projects\\Opencv_SFML_example\\src\\Eater-Regular.ttf")) {
        // Handle error
    }

    std::vector<std::string> mainItems = {"Start Game", "High Scores", "Exit"};
    for (size_t i = 0; i < mainItems.size(); ++i) {
        sf::Text text;
        text.setFont(font_);
        text.setString(mainItems[i]);
        text.setCharacterSize(30);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - text.getGlobalBounds().width / 2, height / (mainItems.size() + 1) * (i + 1)));
        mainMenuItems_.push_back(text);
    }

    std::vector<std::string> difficultyItems = {"Easy", "Medium", "Hard"};
    for (size_t i = 0; i < difficultyItems.size(); ++i) {
        sf::Text text;
        text.setFont(font_);
        text.setString(difficultyItems[i]);
        text.setCharacterSize(30);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - text.getGlobalBounds().width / 2, height / (difficultyItems.size() + 1) * (i + 1)));
        difficultyMenuItems_.push_back(text);
    }
}

void Menu::draw(sf::RenderWindow &window) {
    const auto& items = (state_ == MainMenu) ? mainMenuItems_ : difficultyMenuItems_;
    for (const auto &item : items) {
        window.draw(item);
    }
}

void Menu::selectItem(int index) {
    auto& items = (state_ == MainMenu) ? mainMenuItems_ : difficultyMenuItems_;
    if (index >= 0 && index < items.size()) {
        items[selectedItemIndex_].setFillColor(sf::Color::White);
        selectedItemIndex_ = index;
        items[selectedItemIndex_].setFillColor(sf::Color::Red);
    }
}

int Menu::getSelectedItemIndex() const {
    return selectedItemIndex_;
}

void Menu::setState(State state) {
    state_ = state;
    selectedItemIndex_ = 0;
    auto& items = (state_ == MainMenu) ? mainMenuItems_ : difficultyMenuItems_;
    for (auto& item : items) {
        item.setFillColor(sf::Color::White);
    }
    items[selectedItemIndex_].setFillColor(sf::Color::Red);
}

Menu::State Menu::getState() const {
    return state_;
}