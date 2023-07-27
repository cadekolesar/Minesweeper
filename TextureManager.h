//
// Created by Caden Kolesar on 4/11/21.
//
#ifndef COP3503PROJECT3_TEXTUREMANAGER_H
#define COP3503PROJECT3_TEXTUREMANAGER_H

#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace std;

class TextureManager {
    static unordered_map<string , sf::Texture> textures;
    static void LoadTexture(string fileName);
public:
    static sf::Texture& GetTexture(string textureName);
    static void Clear();
};


#endif //COP3503PROJECT3_TEXTUREMANAGER_H
