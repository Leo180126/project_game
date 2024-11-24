#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include <string>
class HighScore{
    public:
        static int loadHighScore(const std::string& filename);
        static void saveHighScore(const std::string& filename,int score);
};
#endif