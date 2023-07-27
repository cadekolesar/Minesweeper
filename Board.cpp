//
// Created by Caden Kolesar on 4/11/21.
//
#include "Board.h"
#include <iostream>
#include <fstream>
#include <set>
#include <random>
using namespace std;
//width == x
//length == y
void Board::loadFile(string fileName) {
    gameLost = false;
    gameWon = false;
    ifstream file;
    file.open("boards/config.cfg");
    if(file.is_open()){
        file >> this->width;
        file >> this->length;
        file >> this->numMines;
    }
    file.close();
    file.open("boards/testboard" + fileName + ".brd");
    myVect.clear();
    if(file.is_open()){
        char curr;
        while(!file.eof()){
            Tile tile;
            curr = file.get();
            if(curr == '\r'){
                curr = file.get();
            }
            if(curr == '\n'){
                curr = file.get();
            }
            if(curr == '1') {
                tile.setMine(true);
            }
            else{
                tile.setMine(false);
            }
            for(int k = 0; k < 8; k++){
                tile.adjacentMines[k] = false;
            }
            myVect.push_back(tile);
            if(myVect.size() == Board::GetWidth() * Board::GetLength()){
                break;
            }
        }
        vector<Tile> vect2;
        for(int i = 0; i < width; i++){
            for(int j = 0; j < length; j++){
                vect2.push_back(myVect.at(j * width + i));
            }
        }
        myVect = vect2;
    }
}

Board::Board() {
    loadFileRandom();
}

int Board::GetWidth() {
    return this->width;
}

int Board::GetLength() {
    return this->length;
}

int Board::GetNumMines() {
    return this->numMines;
}

void Board::SetBoard(sf::RenderWindow &window) {
    float x, y;
    int count = 0;
    numTilesRevealed = 0;
    for (unsigned int i = 0; i < width; i++) {
        x = i * 32;
        y = 0;
        for (unsigned int j = 0; j < length; j++) {
            sf::Sprite bottomTile(TextureManager::GetTexture("tile_revealed"));
            sf::Sprite topTile(TextureManager::GetTexture("tile_hidden"));
            sf::Sprite mine(TextureManager::GetTexture("mine"));
            sf::Sprite flag(TextureManager::GetTexture("flag"));
            sf::Sprite happyFace(TextureManager::GetTexture("face_happy"));
            sf::Sprite winFace(TextureManager::GetTexture("face_win"));
            sf::Sprite loseFace(TextureManager::GetTexture("face_lose"));
            sf::Sprite debug(TextureManager::GetTexture("debug"));
            sf::Sprite test1(TextureManager::GetTexture("test_1"));
            sf::Sprite test2(TextureManager::GetTexture("test_2"));
            sf::Sprite test3(TextureManager::GetTexture("test_3"));
            happyFace.setPosition((width) * 16 - 32 , length * 32);
            winFace.setPosition((width) * 16 - 32, length * 32);
            loseFace.setPosition((width) * 16 - 32, length * 32);
            if(gameLost){
                window.draw(loseFace);
            }
            else if(gameWon){
                window.draw(winFace);
            }
            else{
                window.draw(happyFace);
            }
            debug.setPosition((width) * 16 + 128 - 32, length * 32);
            test1.setPosition((width ) * 16 + 192 - 32, length * 32);
            test2.setPosition((width) * 16 + 256 - 32, length * 32);
            test3.setPosition((width) * 16 + 320 - 32, length * 32);
            window.draw(debug);
            window.draw(test1);
            window.draw(test2);
            window.draw(test3);
            setDigits(window);
            int count2 = 0;
            if(myVect.at(count).removeTop){
                myVect.at(count).showNum = true;
            }
            if (myVect.at(count).showNum) {
                for (int k = 0; k < 8; k++) {
                    if (myVect.at(count).adjacentMines[k]) {
                        count2++;
                    }
                }
            }
            y = j * 32;
                myVect.at(count).x = x;
                myVect.at(count).y = y;
                //bottom tile
                bottomTile.setPosition(x, y);
                window.draw(bottomTile);
                //show Numbers
                if(myVect.at(count).showNum && count2 != 0){
                    sf::Sprite number(TextureManager::GetTexture("number_" + to_string(count2)));
                    //numTilesRevealed++;
                    number.setPosition(x , y);
                    window.draw(number);
                    /*if(numTilesRevealed + numMines == length * width){
                        winGame();
                    }*/
                }
                //top tile
                if(!myVect.at(count).removeTop) {
                    topTile.setPosition(x, y);
                    window.draw(topTile);
                }
                //flag
                if(myVect.at(count).showFlag){
                    flag.setPosition(x, y);
                    topTile.setPosition(x, y);
                    window.draw(topTile);
                    window.draw(flag);
                    numFlags++;
                }
                //mine
                if(myVect.at(count).showMine){
                    mine.setPosition(x, y);
                    window.draw(mine);
                }
            if(myVect.at(count).removeTop){
                numTilesRevealed++;
            }
                count++;
            sprites.clear();
            sprites.push_back(bottomTile);
            sprites.push_back(topTile);
            sprites.push_back(mine);
            sprites.push_back(flag);
            sprites.push_back(happyFace);
            sprites.push_back(winFace);
            sprites.push_back(loseFace);
            sprites.push_back(debug);
            sprites.push_back(test1);
            sprites.push_back(test2);
            sprites.push_back(test3);
            }
        }
}

float Board::findCoordinate(int x , int y) {
    for(int i = 0; i < myVect.size(); i++) {
        if((myVect.at(i).x / 32 == x / 32) && (myVect.at(i).y / 32 == y / 32)){
            return i;
        }
    }
    Tile tile;
    myVect.push_back(tile);
    return myVect.size() - 1;
}

void Board::placeFlag(int index) {
  if(canSetFlags) {
      if (myVect.at(index).showFlag) {
          myVect.at(index).showFlag = false;
          myVect.at(index).removeTop = false;
      } else {
          myVect.at(index).showFlag = true;
          myVect.at(index).removeTop = false;
      }
  }
}

void Board::reveal(int index) {
    if(myVect.at(index).mine && !myVect.at(index).showFlag){
        myVect.at(index).showMine = true;
    }
    else{
        myVect.at(index).removeTop = true;
        myVect.at(index).showNum = true;
        int count = 0;
        for(int i = 0; i < 8; i++){
            if(myVect.at(index).adjacentMines[i]){
                count++;
            }
        }
        if(count == 0){
            myVect.at(index).showNum = false;
            recursiveReveal(index);
        }
    }
    numTilesRevealed = 0;
    for(int i = 0; i < myVect.size(); i++){
        if(myVect.at(i).removeTop){
            numTilesRevealed++;
        }
    }
    winGame();
}

void Board::connectMines() {
    for (int i = 0; i < myVect.size(); i++) {
        if (!myVect.at(i).mine) {
            //top left corner
            if (((myVect.at(i).x) == 0) && ((myVect.at(i).y) == 0)) {
                if (myVect.at(i + 1).mine) {
                    myVect.at(i).adjacentMines[6] = true;
                }
                if (myVect.at(i + (length)).mine) {
                    myVect.at(i).adjacentMines[5] = true;
                }
                if(myVect.at(i + length + 1).mine){
                    myVect.at(i).adjacentMines[7] = true;
                }
            }
                //top right corner
            else if (((myVect.at(i).x / 32) == (width - 1)) && ((myVect.at(i).y / 32) == 0)) {
                if (myVect.at(i - length).mine) {
                    myVect.at(i).adjacentMines[3] = true;
                }
                if (myVect.at(i + 1).mine) {
                    myVect.at(i).adjacentMines[6] = true;
                }
                if(myVect.at(i - length + 1).mine){
                    myVect.at(i).adjacentMines[5] = true;
                }
            }
                //bottom left corner
            else if (((myVect.at(i).x / 32) == 0) && ((myVect.at(i).y / 32) == (length - 1))) {
                if (myVect.at(i - 1).mine) {
                    myVect.at(i).adjacentMines[1] = true;
                }
                if (myVect.at(i + length).mine) {
                    myVect.at(i).adjacentMines[4] = true;
                }
                if(myVect.at(i + length - 1).mine){
                    myVect.at(i).adjacentMines[2] = true;
                }
            }
                //bottom right corner
            else if (((myVect.at(i).x / 32) == (width - 1)) && ((myVect.at(i).y / 32) == (length - 1))) {
                if (myVect.at(i - 1).mine) {
                    myVect.at(i).adjacentMines[1] = true;
                }
                if (myVect.at(i - (length)).mine) {
                    myVect.at(i).adjacentMines[3] = true;
                }
                if(myVect.at(i - length - 1).mine){
                    myVect.at(i).adjacentMines[0] = true;
                }
            }
                //left side
            else if (myVect.at(i).x / 32 == 0) {
                if(myVect.at(i - 1).mine){
                    myVect.at(i).adjacentMines[1] = true;
                }
                if(myVect.at((i + length) - 1).mine){
                    myVect.at(i).adjacentMines[2] = true;
                }
                if(myVect.at(i + length).mine){
                    myVect.at(i).adjacentMines[4] = true;
                }
                if(myVect.at(i + 1).mine){
                    myVect.at(i).adjacentMines[6] = true;
                }
                if(myVect.at((i + length) + 1).mine){
                    myVect.at(i).adjacentMines[7] = true;
                }
            }
                //right side
            else if (myVect.at(i).x / 32 == width - 1) {
                if(myVect.at((i - length) - 1).mine){
                    myVect.at(i).adjacentMines[0] = true;
                }
                if(myVect.at((i - 1)).mine){
                    myVect.at(i).adjacentMines[1] = true;
                }
                if(myVect.at(i - length).mine){
                    myVect.at(i).adjacentMines[3] = true;
                }
                if(myVect.at((i - length) + 1).mine){
                    myVect.at(i).adjacentMines[5] = true;
                }
                if(myVect.at((i + 1)).mine){
                    myVect.at(i).adjacentMines[6] = true;
                }
            }
                //top row
            else if (myVect.at(i).y / 32 == 0) {
                if(myVect.at(i - length).mine){
                    myVect.at(i).adjacentMines[3] = true;
                }
                if(myVect.at(i  + length).mine){
                    myVect.at(i).adjacentMines[4] = true;
                }
                if(myVect.at((i - length + 1)).mine){
                    myVect.at(i).adjacentMines[5] = true;
                }
                if(myVect.at(i + 1).mine){
                    myVect.at(i).adjacentMines[6] = true;
                }
                if(myVect.at((i + length) + 1).mine){
                    myVect.at(i).adjacentMines[7] = true;
                }
            }
                //bottom row
            else if (myVect.at(i).y / 32 == length - 1) {
                if(myVect.at(i - length - 1).mine){
                    myVect.at(i).adjacentMines[0] = true;
                }
                if(myVect.at(i - 1).mine){
                    myVect.at(i).adjacentMines[1] = true;
                }
                if(myVect.at((i + length) - 1).mine){
                    myVect.at(i).adjacentMines[2] = true;
                }
                if(myVect.at(i - length).mine){
                    myVect.at(i).adjacentMines[4] = true;
                }
                if(myVect.at((i + length)).mine){
                    myVect.at(i).adjacentMines[5] = true;
                }
            }
                //every other tile
            else {
                if(myVect.at((i - length) - 1).mine){
                    myVect.at(i).adjacentMines[0] = true;
                }
                if(myVect.at((i - 1)).mine){
                    myVect.at(i).adjacentMines[1] = true;
                }
                if(myVect.at((i + length - 1)).mine){
                    myVect.at(i).adjacentMines[2] = true;
                }
                if(myVect.at(i - length).mine){
                    myVect.at(i).adjacentMines[3] = true;
                }
                if(myVect.at(i + length).mine){
                    myVect.at(i).adjacentMines[4] = true;
                }
                if(myVect.at((i - length + 1)).mine){
                    myVect.at(i).adjacentMines[5] = true;
                }
                if(myVect.at((i + 1)).mine){
                    myVect.at(i).adjacentMines[6] = true;
                }
                if(myVect.at((i + length + 1)).mine){
                    myVect.at(i).adjacentMines[7] = true;
                }
            }
        }
    }
}

bool Board::loseGame(int index) {
    if(myVect.at(index).mine && !myVect.at(index).showFlag){
        return true;
    }
    else{
        return false;
    }
}

bool Board::winGame() {
    if((length * width) - numTilesRevealed <= numMines){
        return true;
    }
    else{
        return false;
    }
}

void Board::reset() {

}

void Board::debug() {
    for(int i = 0; i < myVect.size(); i++){
        if(myVect.at(i).mine) {
            myVect.at(i).showMine = true;
        }
    }
}

void Board::bug() {
    for(int i = 0; i < myVect.size(); i++){
        if(myVect.at(i).mine) {
            myVect.at(i).removeTop = false;
            myVect.at(i).showMine = false;
        }
    }
}

void Board::loadFileRandom() {
    gameLost = false;
    gameWon = false;
    ifstream file;
    file.open("boards/config.cfg");
    if(file.is_open()){
        file >> this->width;
        file >> this->length;
        file >> this->numMines;
    }
    myVect.clear();
    file.close();
    std::mt19937 random(time(0));
    std::uniform_int_distribution<int> dist(0 , width * length);
    set<int> randomNums;
    while(randomNums.size() < numMines) {
        randomNums.insert(dist(random));
    }
    for(int i = 0; i < width * length; i++) {
        Tile tile;
        if(randomNums.count(i) == 1) {
            tile.mine = true;
        }
        else {
            tile.mine = false;
        }
        for(int k = 0; k < 8; k++){
            tile.adjacentMines[k] = false;
        }
        myVect.push_back(tile);
    }

        vector<Tile> vect2;
        for(int i = 0; i < width; i++){
            for(int j = 0; j < length; j++){
                vect2.push_back(myVect.at(j * width + i));
            }
        }
        myVect = vect2;
    }

void Board::setDigits(sf::RenderWindow &window) {
    int count = 0;
    for(unsigned int i = 0; i < myVect.size(); i++){
        if(myVect.at(i).showFlag){
            count++;
        }
    }
    int numToShow = numMines - count;
    if(numToShow < 0){
        numToShow = -numToShow;
    }
    int hundreds = (numToShow) / 100;
    int tens = ((numToShow) - (hundreds * 100)) / 10;
    int ones = ((numToShow) - (hundreds * 100 + tens * 10));
    sf::Sprite digits(TextureManager::GetTexture("digits"));
    if((numMines - count) < 0){
        digits.setTextureRect(sf::IntRect(21 * 10 , 0, 21 ,32));
        digits.setPosition(0 , length * 32);
        window.draw(digits);
    }
    else{
        digits.setTextureRect(sf::IntRect(21 * 0 , 0, 21 ,32));
        digits.setPosition(0 , length * 32);
        window.draw(digits);
    }
    digits.setTextureRect(sf::IntRect(21 * hundreds , 0 , 21 , 32));
    digits.setPosition(21 , length * 32);
    window.draw(digits);
    digits.setTextureRect(sf::IntRect(21 * tens , 0 , 21 , 32));
    digits.setPosition(42 , length * 32);
    window.draw(digits);
    digits.setTextureRect(sf::IntRect(21 * ones , 0 , 21 , 32));
    digits.setPosition(63 , length * 32);
    window.draw(digits);
}

void Board::recursiveReveal(int i) {
    int count = 0;
    for(int j = 0; j < 8; j++){
        if(myVect.at(i).adjacentMines[j]){
            count++;
        }
        if(count > 0){
            myVect.at(i).showNum = true;
            myVect.at(i).removeTop = true;
            myVect.at(i).recursiveCheck = true;
        }
    }
    if(!myVect.at(i).recursiveCheck){
        myVect.at(i).recursiveCheck = true;
    if (((myVect.at(i).x) == 0) && ((myVect.at(i).y) == 0)) {
        if (!myVect.at(i + 1).mine && !myVect.at((i + 1)).showFlag) {
            myVect.at(i + 1).removeTop = true;
            recursiveReveal(i + 1);
        }
        if (!myVect.at(i + (length)).mine && !myVect.at((i + length)).showFlag) {
            myVect.at(i + length).removeTop = true;
            recursiveReveal(i + length);
        }
        if (!myVect.at(i + length + 1).mine && !myVect.at((i + length) + 1).showFlag) {
            myVect.at(i + length + 1).removeTop = true;
            recursiveReveal(i + length + 1);
        }
    }
        //top right corner
    else if (((myVect.at(i).x / 32) == (width - 1)) && ((myVect.at(i).y / 32) == 0)) {
        if (!myVect.at(i - length).mine && !myVect.at((i - length)).showFlag) {
            myVect.at(i - length).removeTop = true;
            recursiveReveal(i - length);
        }
        if (!myVect.at(i + 1).mine && !myVect.at((i + 1)).showFlag) {
            myVect.at(i + 1).removeTop = true;
            recursiveReveal(i + 1);
        }
        if (!myVect.at(i - length + 1).mine && !myVect.at((i - length) + 1).showFlag) {
            myVect.at(i - length + 1).removeTop = true;
            recursiveReveal(i - length + 1);
        }
    }
        //bottom left corner
    else if (((myVect.at(i).x / 32) == 0) && ((myVect.at(i).y / 32) == (length - 1))) {
        if (!myVect.at(i - 1).mine && !myVect.at((i - 1)).showFlag) {
            myVect.at(i - 1).removeTop = true;
            recursiveReveal(i - 1);
        }
        if (!myVect.at(i + length).mine && !myVect.at((i + length)).showFlag) {
            myVect.at(i + length).removeTop = true;
            recursiveReveal(i + length);
        }
        if (!myVect.at(i + length - 1).mine && !myVect.at((i + length) - 1).showFlag) {
            myVect.at(i + length - 1).removeTop = true;
            recursiveReveal(i + length - 1);
        }
    }
        //bottom right corner
    else if (((myVect.at(i).x / 32) == (width - 1)) && ((myVect.at(i).y / 32) == (length - 1))) {
        if (!myVect.at(i - 1).mine && !myVect.at((i - 1)).showFlag) {
            myVect.at(i - 1).removeTop = true;
            recursiveReveal(i - 1);
        }
        if (!myVect.at(i - (length)).mine && !myVect.at((i - length)).showFlag) {
            myVect.at(i - length).removeTop = true;
            recursiveReveal(i - length);
        }
        if (!myVect.at(i - length - 1).mine && !myVect.at((i - length) - 1).showFlag) {
            myVect.at(i - length - 1).removeTop = true;
            recursiveReveal(i - length - 1);
        }
    }
        //left side
    else if (myVect.at(i).x / 32 == 0) {
        if (!myVect.at(i - 1).mine && !myVect.at((i - 1)).showFlag) {
            myVect.at(i - 1).removeTop = true;
            recursiveReveal(i - 1);
        }
        if (!myVect.at((i + length) - 1).mine && !myVect.at((i + length) - 1).showFlag) {
            myVect.at(i + length - 1).removeTop = true;
            recursiveReveal(i + length - 1);
        }
        if (!myVect.at(i + length).mine && !myVect.at((i + length)).showFlag) {
            myVect.at(i + length).removeTop = true;
            recursiveReveal(i + length);
        }
        if (!myVect.at(i + 1).mine && !myVect.at((i + 1)).showFlag) {
            myVect.at(i + 1).removeTop = true;
            recursiveReveal(i + 1);
        }
        if (!myVect.at((i + length) + 1).mine && !myVect.at((i + length) + 1).showFlag) {
            myVect.at(i + length + 1).removeTop = true;
            recursiveReveal(i + length + 1);
        }
    }
        //right side
    else if (myVect.at(i).x / 32 == width - 1) {
        if (!myVect.at((i - length) - 1).mine && !myVect.at((i - length) - 1).showFlag) {
            myVect.at(i - length - 1).removeTop = true;
            recursiveReveal(i - length - 1);
        }
        if (!myVect.at((i - 1)).mine && !myVect.at((i - 1)).showFlag) {
            myVect.at(i - 1).removeTop = true;
            recursiveReveal(i - 1);
        }
        if (!myVect.at(i - length).mine && !myVect.at((i - length)).showFlag) {
            myVect.at(i - length).removeTop = true;
            recursiveReveal(i - length);
        }
        if (!myVect.at((i - length) + 1).mine && !myVect.at((i - length) + 1).showFlag) {
            myVect.at(i - length + 1).removeTop = true;
            recursiveReveal(i - length + 1);
        }
        if (!myVect.at((i + 1)).mine && !myVect.at((i + 1)).showFlag) {
            myVect.at(i + 1).removeTop = true;
            recursiveReveal(i + 1);
        }
    }
        //top row
    else if (myVect.at(i).y / 32 == 0) {
        if (!myVect.at(i - length).mine && !myVect.at((i - length)).showFlag) {
            myVect.at(i - length).removeTop = true;
            recursiveReveal(i - length);
        }
        if (!myVect.at(i + length).mine && !myVect.at((i + length)).showFlag) {
            myVect.at(i + length).removeTop = true;
            recursiveReveal(i + length);
        }
        if (!myVect.at((i - length + 1)).mine && !myVect.at((i - length) + 1).showFlag) {
            myVect.at(i - length + 1).removeTop = true;
            recursiveReveal(i - length + 1);
        }
        if (!myVect.at(i + 1).mine && !myVect.at((i) + 1).showFlag) {
            myVect.at(i + 1).removeTop = true;
            recursiveReveal(i + 1);
        }
        if (!myVect.at((i + length) + 1).mine && !myVect.at((i + length) + 1).showFlag) {
            myVect.at(i + length + 1).removeTop = true;
            recursiveReveal(i + length + 1);
        }
    }
        //bottom row
    else if (myVect.at(i).y / 32 == length - 1) {
        if (!myVect.at(i - length - 1).mine && !myVect.at((i - length) - 1).showFlag) {
            myVect.at(i - length - 1).removeTop = true;
            recursiveReveal(i - length - 1);
        }
        if (!myVect.at(i - 1).mine && !myVect.at((i - 1)).showFlag) {
            myVect.at(i - 1).removeTop = true;
            recursiveReveal(i - 1);
        }
        if (!myVect.at((i + length) - 1).mine && !myVect.at((i + length) - 1).showFlag) {
            myVect.at((i + length) - 1).removeTop = true;
            recursiveReveal(i + length - 1);
        }
        if (!myVect.at(i - length).mine && !myVect.at((i - length)).showFlag) {
            myVect.at(i - length).removeTop = true;
            recursiveReveal(i - length);
        }
        if (!myVect.at((i + length)).mine && !myVect.at((i + length)).showFlag) {
            myVect.at(i + length).removeTop = true;
            recursiveReveal(i + length);
        }
    }
    else {
        if (!myVect.at((i - length) - 1).mine && !myVect.at((i - length) - 1).showFlag) {
            myVect.at((i - length) - 1).removeTop = true;
            recursiveReveal(i - length - 1);
        }
        if (!myVect.at((i - 1)).mine && !myVect.at((i - 1)).showFlag) {
            myVect.at((i - 1)).removeTop = true;
            recursiveReveal(i - 1);
        }
        if (!myVect.at((i + length - 1)).mine && !myVect.at((i + length) - 1).showFlag) {
            myVect.at((i + length) - 1).removeTop = true;
            recursiveReveal(i + length - 1);
        }
        if (!myVect.at(i - length).mine && !myVect.at((i - length)).showFlag) {
            myVect.at((i - length)).removeTop = true;
            recursiveReveal(i - length);
        }
        if (!myVect.at(i + length).mine && !myVect.at((i + length)).showFlag) {
            myVect.at((i + length)).removeTop = true;
            recursiveReveal(i + length);
        }
        if (!myVect.at((i - length + 1)).mine && !myVect.at((i - length) + 1).showFlag) {
            myVect.at((i - length + 1)).removeTop = true;
            recursiveReveal(i - length + 1);
        }
        if (!myVect.at((i + 1)).mine && !myVect.at((i + 1)).showFlag) {
            myVect.at((i + 1)).removeTop = true;
            recursiveReveal(i + 1);
        }
        if (!myVect.at((i + length + 1)).mine && !myVect.at((i + length) + 1).showFlag) {
            myVect.at((i + length) + 1).removeTop = true;
            recursiveReveal(i + length + 1);
        }
    }
    }
}

