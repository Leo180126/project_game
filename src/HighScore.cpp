#include "HighScore.h"
#include <fstream>
#include <iostream>
int HighScore::loadHighScore(const std::string& filename){
    std::ifstream file (filename);
    int highScore=0;
    if(file.is_open()){
        file >> highScore;
        file.close();
    }
    return highScore;
}
void HighScore::saveHighScore(const std::string& filename,int score){
    std::ofstream file(filename);
    if(file.is_open()){
        file << score;
        file.close();
    }
}