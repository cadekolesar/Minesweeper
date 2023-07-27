//
// Created by Caden Kolesar on 4/11/21.
//

#ifndef COP3503PROJECT3_TILE_H
#define COP3503PROJECT3_TILE_H
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
class Tile {
public:
    Tile();
    void findMines();
    void setMine(bool mine);
    bool showMine = false;
    bool showNum = false;
    bool showFlag = false;
    bool removeTop = false;
    bool adjacentMines[8];
    float x , y;
    bool mine;
    bool recursiveCheck = false;
};


#endif //COP3503PROJECT3_TILE_H
