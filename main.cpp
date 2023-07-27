#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Board.h"

int main() {
    Board board;
    int count = 0;
    int count2 = 0;
    int countWins = 0;
    bool toggle = false;
    bool debugButton = false;
    sf::RenderWindow window(sf::VideoMode(board.GetWidth() * 32, (board.GetLength() * 32) + 88), "My window",
                            sf::Style::Default);
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        int index;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        index = board.findCoordinate(event.mouseButton.x, event.mouseButton.y);
                        board.placeFlag(index);
                    } else if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Sprite debug = board.sprites.at(7);
                        sf::Sprite test1 = board.sprites.at(8);
                        sf::Sprite test2 = board.sprites.at(9);
                        sf::Sprite test3 = board.sprites.at(10);
                        sf::Sprite happyFace = board.sprites.at(4);
                        sf::Sprite winFace = board.sprites.at(5);
                        sf::Sprite loseFace = board.sprites.at(6);
                        if(countWins == 0 && board.winGame()) {
                            board.gameWon = true;
                            board.canSetFlags = false;
                            count2++;
                            countWins++;
                            board.reveal(board.findCoordinate(event.mouseButton.x , event.mouseButton.y));
                            for(int i = 0; i < board.myVect.size(); i++){
                                if(board.myVect.at(i).mine){
                                    board.myVect.at(i).showFlag = true;
                                    board.myVect.at(i).showMine = false;
                                }
                            }
                            if(((winFace.getPosition().x <= event.mouseButton.x)) &&
                                ((winFace.getPosition().x + 64 >= event.mouseButton.x)) &&
                                ((winFace.getPosition().y <= event.mouseButton.y)) &&
                                ((winFace.getPosition().y + 64 >= event.mouseButton.y))) {
                                board.loadFileRandom();
                                board.gameWon = false;
                                count2 = 0;
                            }
                        }
                        else if(((count != 0 && board.loseGame(board.findCoordinate(event.mouseButton.x , event.mouseButton.y))))){
                            board.gameLost = true;
                            board.canSetFlags = false;
                            count2++;
                            for(int i = 0; i < board.myVect.size(); i++){
                                if(board.myVect.at(i).mine){
                                    board.myVect.at(i).showMine = true;
                                }
                            }
                            if((loseFace.getPosition().x <= event.mouseButton.x) &&
                               (loseFace.getPosition().x + 64 >= event.mouseButton.x) &&
                               (loseFace.getPosition().y <= event.mouseButton.y) &&
                               (loseFace.getPosition().y + 64 >= event.mouseButton.y)){
                                board.loadFileRandom();
                                board.gameLost = false;
                                count2 = 0;
                            }
                        }
                        else if ((debug.getPosition().x <= event.mouseButton.x) &&
                            (debug.getPosition().x + 64 >= event.mouseButton.x) &&
                            (debug.getPosition().y <= event.mouseButton.y) &&
                            (debug.getPosition().y + 64 >= event.mouseButton.y) && !board.gameLost) {
                            if (debugButton) {
                                board.bug();
                                debugButton = false;
                            } else {
                                board.debug();
                                debugButton = true;
                            }
                        } else if ((test1.getPosition().x <= event.mouseButton.x) &&
                                   (test1.getPosition().x + 64 >= event.mouseButton.x) &&
                                   (test1.getPosition().y <= event.mouseButton.y) &&
                                   (test1.getPosition().y + 64 >= event.mouseButton.y)) {
                            board.loadFile("1");
                            debugButton = false;

                            count2 = 0;
                            board.canSetFlags = true;
                            count = 0;
                            countWins = 0;
                        } else if ((test2.getPosition().x <= event.mouseButton.x) &&
                                   (test2.getPosition().x + 64 >= event.mouseButton.x) &&
                                   (test2.getPosition().y <= event.mouseButton.y) &&
                                   (test2.getPosition().y + 64 >= event.mouseButton.y)) {
                            board.loadFile("2");
                            debugButton = false;
                            count2 = 0;
                            toggle = true;
                            board.canSetFlags = true;
                            count = 0;
                            countWins = 0;
                        } else if ((test3.getPosition().x <= event.mouseButton.x) &&
                                   (test3.getPosition().x + 64 >= event.mouseButton.x) &&
                                   (test3.getPosition().y <= event.mouseButton.y) &&
                                   (test3.getPosition().y + 64 >= event.mouseButton.y)) {
                            board.loadFile("3");
                            debugButton = false;
                            count2 = 0;
                            board.canSetFlags = true;
                            count = 0;
                            countWins = 0;
                        } else if ((happyFace.getPosition().x <= event.mouseButton.x) &&
                                   (happyFace.getPosition().x + 64 >= event.mouseButton.x) &&
                                   (happyFace.getPosition().y <= event.mouseButton.y) &&
                                   (happyFace.getPosition().y + 64 >= event.mouseButton.y)) {
                            board.loadFileRandom();
                            debugButton = false;
                            count = 0;
                            count2 = 0;
                            board.canSetFlags = true;
                        }
                        else if(count2 == 0){
                            index = board.findCoordinate(event.mouseButton.x, event.mouseButton.y);
                            board.reveal(index);
                            if (index == board.GetLength() * board.GetWidth()) {
                                board.myVect.pop_back();
                            }
                        }
                    }
                }
            }
            board.SetBoard((sf::RenderWindow &) window);
            if (count == 0) {
                board.connectMines();
            }
        if(board.winGame()) {
            board.gameWon = true;
            board.canSetFlags = false;
            count2++;
            countWins++;
            count = 0;
            board.reveal(board.findCoordinate(event.mouseButton.x, event.mouseButton.y));
            for (int i = 0; i < board.myVect.size(); i++) {
                if (board.myVect.at(i).mine) {
                    board.myVect.at(i).showFlag = true;
                    board.myVect.at(i).showMine = false;
                }
            }
        }
            window.display();
            count++;
        }
        TextureManager::Clear();
        return 0;
    }
