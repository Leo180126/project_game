#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "Direction.h"
#include "Game.h"
#include "HighScore.h"
#include "FingerDetector.h"

Game::Game(sf::RenderWindow* window)
    : window_(window),
      tileEdgeLength_{20.f},
      isPaused_(false),
      highScore_(0),
      menu_(window->getSize().x, window->getSize().y),
      isMenuActive_(true),
      difficultyLevel_(0) // Default to Easy
{
    // Window height and width should be multiple of tile edge length
    assert(!(window_->getSize().y % (int)tileEdgeLength_) &&
           !(window_->getSize().x % (int)tileEdgeLength_));
    window_->setFramerateLimit(60);

    // Load textures from config.txt
    std::ifstream configFile("C:\\Users\\84333\\projects\\Opencv_SFML_example\\src\\config.txt");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config.txt" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Get path of png file line to line from config file
    std::string line;
    while (std::getline(configFile, line)) {
        size_t delimiterPos = line.find('=');
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        auto texture = std::make_unique<sf::Texture>();
        if (!texture->loadFromFile(value)) {
            std::cerr << "Failed to load texture: " << value << std::endl;
            exit(EXIT_FAILURE);
        }

        if (key == "fieldTexture") {
            textures_[0] = std::move(texture);
        } else if (key == "snakeTexture") {
            textures_[1] = std::move(texture);
        } else if (key == "foodTexture") {
            textures_[2] = std::move(texture);
        }
    }
    configFile.close();

    // Initialize game components
    field_ = std::make_unique<GameField>(window_->getSize().x / tileEdgeLength_,
                                         window_->getSize().y / tileEdgeLength_,
                                         tileEdgeLength_,
                                         textures_[0].get());

    snake_ = std::make_unique<Snake>(field_.get(), textures_[1].get(), 0.1f);
    food_ = std::make_unique<FoodSpawner>(field_.get(), textures_[2].get());

    // Load high score
    highScore_ = HighScore::loadHighScore("highscore.txt");
}

Game::~Game() = default;

void Game::run()
{
    Direction::Type directionChosen = Direction::None;

    // OpenCV setup for webcam capture
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera" << std::endl;
        return;
    }

    bool gameRunning = true;

    // Move the OpenCV window to a different position
    cv::namedWindow("Webcam Feed", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Webcam Feed", 800, 0); // Move to (800, 0) on the screen

    FingerDetector fingerDetector;

    while (window_->isOpen())
    {
        sf::Event event;
        while (window_->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window_->close();
            }
            if (event.type == sf::Event::LostFocus) {
                isPaused_ = true;
            }
            if (event.type == sf::Event::GainedFocus) {
                isPaused_ = false;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    isPaused_ = !isPaused_;
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    std::cout << "Escape key pressed" << std::endl;
                }
                if (isMenuActive_) {
                    if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num3) {
                        int selectedItem = event.key.code - sf::Keyboard::Num1;
                        if (menu_.getState() == Menu::MainMenu) {
                            menu_.selectItem(selectedItem);
                            if (selectedItem == 0) {
                                menu_.setState(Menu::DifficultyMenu); // Go to Difficulty Menu
                            } else if (selectedItem == 1) {
                                // Show High Scores (implement as needed)
                            } else if (selectedItem == 2) {
                                window_->close(); // Exit
                            }
                        } else if (menu_.getState() == Menu::DifficultyMenu) {
                            menu_.selectItem(selectedItem);
                            difficultyLevel_ = selectedItem; // Set difficulty level
                            isMenuActive_ = false; // Start Game
                        }
                    }
                }
            }
        }

        if (isMenuActive_) {
            window_->clear();
            menu_.draw(*window_);
            window_->display();
        } else {
            if (!isPaused_) {
                // Update game logic
                if (gameRunning) {
                    // Capture frame from the webcam
                    cv::Mat frame;
                    cap >> frame;
                    if (frame.empty()) break;

                    // Detect fingertip
                    cv::Point fingertip = fingerDetector.detectFingertip(frame);

                    if (fingertip.x != -1 && fingertip.y != -1) {
                        // Update direction based on fingertip position
                        directionChosen = Direction::fromFingertipPosition(fingertip);
                    }

                    cv::imshow("Webcam Feed", frame);

                    if (cv::waitKey(1) == 'q') break;

                    // Update snake and check game state
                    snake_->update(directionChosen, food_->currentPos());

                    if (snake_->isFoodEaten()) {
                        food_->respawn();
                        highScore_++;
                        window_->setTitle("SnakeGame | Score: " + std::to_string(highScore_));
                        if (highScore_ + 3 == field_->tileSum()) {
                            std::cout << "The end! You won..." << std::endl;
                            gameRunning = false;
                        }
                    }

                    if (snake_->isDead()) {
                        std::cout << "The end! You are dead..." << std::endl;
                        gameRunning = false;
                    }
                }
            }

            // Render game
            window_->clear();
            window_->draw(*field_->getRectToDraw());
            for (auto drawable : snake_->getRectsToDraw())
                window_->draw(*drawable);
            window_->draw(*food_->getRectToDraw());
            window_->display();
        }
    }

    cap.release();
    cv::destroyAllWindows();
}