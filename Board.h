//
// Created by Caden Kolesar on 4/11/21.
//
#ifndef COP3503PROJECT3_BOARD_H
#define COP3503PROJECT3_BOARD_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tile.h"
#include "TextureManager.h"
using namespace std;

class Board {
    int length;
    int width;
    int numMines;
    int numFlags;
public:
    int numTilesRevealed = 0;
    bool canSetFlags = true;
    int vectSize = myVect.size();
    Board();
    int GetWidth();
    int GetLength();
    int GetNumMines();
    void SetBoard(sf::RenderWindow &window);
    float findCoordinate(int x , int y);
    void placeFlag(int index);
    void reveal(int index);
    void connectMines();
    bool loseGame(int index);
    bool winGame();
    void reset();
    void debug();
    void bug();
    std::vector<sf::Sprite> sprites;
    std::vector<Tile> myVect;
    void loadFile(string fileName);
    void loadFileRandom();
    void setDigits(sf::RenderWindow &window);
    void recursiveReveal(int index);
    bool gameWon = false;
    bool gameLost = false;
};


#endif //COP3503PROJECT3_BOARD_H
