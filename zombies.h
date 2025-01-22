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

class Zombie {
public:
    int health;
    int attack;
    Texture texture;
    Sprite sprite;
    bool spawned;
    Clock animation;
    string type;

    // Audios
    Music peaCollision;
    Music zombieEat;

    Zombie(const string& file, int health, int attack, string name) : health(health), attack(attack), type(name) {
        if (!texture.loadFromFile(file)) {
            cout << "Could not load zombie texture." << endl;
            return;
        };
        sprite.setTexture(texture);
        sprite.setPosition(3000, 3000);
        sprite.setScale(1.4f, 1.4f);
        spawned = false;

        // Load music
        peaCollision.openFromFile("Audios/zombiehit.mp3");
        zombieEat.openFromFile("Audios/zombieeat.mp3");


    }

    void setTexture(const string& file) {
        texture.loadFromFile(file);
    }

    virtual void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }

    virtual void setScale(float x, float y) {
        sprite.setScale(x, y);
    }

    virtual void draw(RenderWindow& window) {
        if (health != 0) {
            window.draw(sprite);
        }
    }

    int getHealth() {
        return health;
    }

    void setHealth(int in) {
        health = in;
    }

    int getAttack() {
        return attack;
    }
    void setAttack(int in) {
        attack = in;
    }

    virtual void moveZombie(int x = 10, int y = 0) = 0;

    virtual void eat() = 0;
};

class SimpleZombie : public Zombie {
public:
    SimpleZombie(float scaleX = 1.0f, float scaleY = 1.0f) : Zombie("Images/simple.png", 100, 25, "simple") {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }

    void moveZombie(int x = -3, int y = 0) {
        sprite.move(x, 0); // Move zombies to the left (-2, 0)
        int xPos = static_cast<int>(sprite.getPosition().x);
        if (xPos % 20 >= 0 && xPos % 20 < 4) {
            setTexture("Images/Frames/zombie0.png");
        }
        else if (xPos % 20 >= 4 && xPos % 20 < 8) {
            setTexture("Images/Frames/zombie1.png");
        }
        else if (xPos % 20 >= 8 && xPos % 20 < 12) {
            setTexture("Images/Frames/zombie2.png");
        }
        else if (xPos % 20 >= 12 && xPos % 20 < 16) {
            setTexture("Images/Frames/zombie3.png");
        }
        else {
            setTexture("Images/Frames/zombie4.png");
        }
    }

    void eat() {
        if (animation.getElapsedTime().asMilliseconds() <= 500) {
            setTexture("Images/eat1.png");
        }
        else if (animation.getElapsedTime().asMilliseconds() > 500 && animation.getElapsedTime().asMilliseconds() <= 1000) {
            setTexture("Images/eat2.png");
        }
        else { 
            animation.restart();
        }
    }
};

class FootballZombie : public Zombie {
public:
    FootballZombie(float scaleX = 0.8f, float scaleY = 0.8f) : Zombie("Images/football.png", 200, 40, "football") {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }

    void moveZombie(int x = -5, int y = 0) {
        sprite.move(x, 0);
        int xPos = static_cast<int>(sprite.getPosition().x);
        if (xPos % 20 >= 0 && xPos % 20 < 3) {
            setTexture("Images/FootballZ/FootballZ/1.gif");
        }
        else if (xPos % 20 >= 3 && xPos % 20 < 6) {
            setTexture("Images/FootballZ/FootballZ/2.gif");
        }
        else if (xPos % 20 >= 6 && xPos % 20 < 9) {
            setTexture("Images/FootballZ/FootballZ/3.gif");
        }
        else if (xPos % 20 >= 9 && xPos % 20 < 12) {
            setTexture("Images/FootballZ/FootballZ/4.gif");
        }
        else if (xPos % 20 >= 12 && xPos % 20 < 15) {
            setTexture("Images/FootballZ/FootballZ/5.gif");
        }
        else if (xPos % 20 >= 15 && xPos % 20 < 18) {
            setTexture("Images/FootballZ/FootballZ/6.gif");
        }
        else {
            setTexture("Images/FootballZ/FootballZ/7.gif");
        }
    }

    void eat() {
        if (animation.getElapsedTime().asMilliseconds() <= 500) {
            setTexture("Images/EatFootballZ/1.gif");
        }
        else if (animation.getElapsedTime().asMilliseconds() > 500 && animation.getElapsedTime().asMilliseconds() <= 1000) {
            setTexture("Images/EatFootballZ/2.gif");
        }
        else {
            animation.restart();
        }
    }
};

class DanceZombie : public Zombie {
private:
    Clock moveTimer;
public:
    DanceZombie(float scaleX = 1.0f, float scaleY = 1.0f) : Zombie("Images/dance.png", 200, 40, "dance") {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }

    void moveZombie(int x = -2, int y = 0) {
        if (moveTimer.getElapsedTime().asSeconds() > 7 && moveTimer.getElapsedTime().asSeconds() <= 8) {
            sprite.move(-8, 10);
        }
        else if (moveTimer.getElapsedTime().asSeconds() > 8) {
            moveTimer.restart();
        }
        else {
            sprite.move(x, 0);
            int xPos = static_cast<int>(sprite.getPosition().x);
            if (xPos % 20 >= 0 && xPos % 20 < 2) {
                setTexture("Images/DancingZ/1.gif");
            }
            else if (xPos % 20 >= 2 && xPos % 20 < 4) {
                setTexture("Images/DancingZ/2.gif");
            }
            else if (xPos % 20 >= 4 && xPos % 20 < 6) {
                setTexture("Images/DancingZ/3.gif");
            }
            else if (xPos % 20 >= 6 && xPos % 20 < 8) {
                setTexture("Images/DancingZ/4.gif");
            }
            else if (xPos % 20 >= 8 && xPos % 20 < 10) {
                setTexture("Images/DancingZ/5.gif");
            }
            else if (xPos % 20 >= 10 && xPos % 20 < 12) {
                setTexture("Images/DancingZ/6.gif");
            }
            else if (xPos % 20 >= 12 && xPos % 20 < 14) {
                setTexture("Images/DancingZ/7.gif");
            }
            else if (xPos % 20 >= 14 && xPos % 20 < 16) {
                setTexture("Images/DancingZ/8.gif");
            }
            else if (xPos % 20 >= 16 && xPos % 20 < 18) {
                setTexture("Images/DancingZ/9.gif");
            }
            else {
                setTexture("Images/DancingZ/10.gif");
            }
        }
    }

    void eat() {
        if (animation.getElapsedTime().asMilliseconds() <= 500) {
            setTexture("Images/EatDancingZ/1.gif");
        }
        else if (animation.getElapsedTime().asMilliseconds() > 500 && animation.getElapsedTime().asMilliseconds() <= 1000) {
            setTexture("Images/EatDancingZ/2.gif");
        }
        else {
            animation.restart();
        }
    }
};

class ConeZombie : public Zombie {
public:
    ConeZombie(float scaleX = 0.8f, float scaleY = 0.8f) : Zombie("Images/ConeZ/ConeZ/1.gif", 200, 25, "cone") {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }

    void moveZombie(int x = -3, int y = 0) {
        sprite.move(x, 0);
        int xPos = static_cast<int>(sprite.getPosition().x);
        if (xPos % 16 >= 0 && xPos % 20 < 2) {
            setTexture("Images/ConeZ/ConeZ/1.gif");
        }
        else if (xPos % 16 >= 2 && xPos % 20 < 4) {
            setTexture("Images/ConeZ/ConeZ/2.gif");
        }
        else if (xPos % 16 >= 4 && xPos % 20 < 6) {
            setTexture("Images/ConeZ/ConeZ/3.gif");
        }
        else if (xPos % 16 >= 6 && xPos % 20 < 8) {
            setTexture("Images/ConeZ/ConeZ/4.gif");
        }
        else if (xPos % 16 >= 8 && xPos % 20 < 10) {
            setTexture("Images/ConeZ/ConeZ/5.gif");
        }
        else if (xPos % 16 >= 10 && xPos % 20 < 12) {
            setTexture("Images/ConeZ/ConeZ/6.gif");
        }
        else if (xPos % 16 >= 12 && xPos % 20 < 14) {
            setTexture("Images/ConeZ/ConeZ/7.gif");
        }
        else {
            setTexture("Images/ConeZ/ConeZ/8.gif");
        }
    }

    void eat() {
        if (animation.getElapsedTime().asMilliseconds() <= 500) {
            setTexture("Images/eat1.png");
        }
        else if (animation.getElapsedTime().asMilliseconds() > 500 && animation.getElapsedTime().asMilliseconds() <= 1000) {
            setTexture("Images/eat2.png");
        }
        else {
            animation.restart();
        }
    }
};

class BalloonZombie : public Zombie {
private:
    Clock moveTimer;
public:
    BalloonZombie(float scaleX = 1.0f, float scaleY = 1.0f) : Zombie("Images/balloon/baloon1.png", 150, 30, "balloon") {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }

    void moveZombie(int x = -4, int y = 0) {
        if (y == 0) {
            sprite.move(x, 0);
            int xPos = static_cast<int>(sprite.getPosition().x);
            if (xPos % 9 >= 0 && xPos % 9 < 3) {
                setTexture("Images/balloon/baloon1.png");
            }
            else if (xPos % 9 >= 3 && xPos % 20 < 6) {
                setTexture("Images/balloon/baloon2.png");
            }
            else {
                setTexture("Images/balloon/baloon3.png");
            }
        }
        else {
            sprite.move(x, 0);
            int xPos = static_cast<int>(sprite.getPosition().x);
            if (xPos % 16 >= 0 && xPos % 20 < 2) {
                setTexture("Images/balloonground/1.png");
            }
            else if (xPos % 16 >= 2 && xPos % 20 < 4) {
                setTexture("Images/balloonground/2.png");
            }
            else if (xPos % 16 >= 4 && xPos % 20 < 6) {
                setTexture("Images/balloonground/3.png");
            }
            else if (xPos % 16 >= 6 && xPos % 20 < 8) {
                setTexture("Images/balloonground/4.png");
            }
            else if (xPos % 16 >= 8 && xPos % 20 < 10) {
                setTexture("Images/balloonground/5.png");
            }
            else if (xPos % 16 >= 10 && xPos % 20 < 12) {
                setTexture("Images/balloonground/6.png");
            }
            else if (xPos % 16 >= 12 && xPos % 20 < 14) {
                setTexture("Images/balloonground/7.png");
            }
            else {
                setTexture("Images/balloonground/8.png");
            }
        }
    }

    void eat() {
        if (animation.getElapsedTime().asMilliseconds() <= 500) {
            setTexture("Images/ballooneat/1.png");
        }
        else if (animation.getElapsedTime().asMilliseconds() > 500 && animation.getElapsedTime().asMilliseconds() <= 1000) {
            setTexture("Images/ballooneat/2.png");
        }
        else {
            animation.restart();
        }
    }
};
