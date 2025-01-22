#pragma once

#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "linker.h"
using namespace sf;
using namespace std;

class LawnMower {
public:
    int attack;
    Texture texture;
    Sprite sprite;
    bool spawned;

    // Audios
    Music lawnMowerStart;

    LawnMower() {
        texture.loadFromFile("Images/car.png");
        sprite.setTexture(texture);
        sprite.setScale(1.4f, 1.4f);

        spawned = true;
        attack = 1000;

        lawnMowerStart.openFromFile("Audios/lawnmower.mp3");
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
    }

    void draw(RenderWindow& window) {
        if (sprite.getPosition().x <= window.getSize().x) {
            window.draw(sprite);
        }
    }

    void move(RenderWindow& window) {
        while (sprite.getPosition().x <= window.getSize().x) {
            sprite.move(30, 0);
        }
    }

    int getAttack() {
        return attack;
    }
};
