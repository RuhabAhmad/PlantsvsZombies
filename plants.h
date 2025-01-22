#pragma once
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

class Plant {
public:
    string type;
    int health;
    int cost;
    int attack;
    Texture texture;
    Sprite sprite;
    bool planted;

    Plant(const string& file, const string& t, int health, int attack, int cost) : type(t), health(health), attack(attack), cost(cost) {
        texture.loadFromFile(file);
        sprite.setTexture(texture);
        sprite.setScale(1.4f, 1.4f); // Adjust scale as needed
        planted = false;
    }

    virtual void dummy() {};

    virtual void abstract() = 0;

    virtual void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    virtual void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    int getCost() {
        return cost;
    }
    void setCost(int in) {
        cost = in;
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

    virtual ~Plant() {}

    #define MAX_SUNLIGHTS 5 // Maximum number of sunlights
    #define MAX_PLANTS 20 // Maximum number of plants
    #define MAX_PEA 200 // Maximum number of peas
    #define MAX_SUNLIGHT 100 // Maximum number of sunlight instances
};

struct InventoryItem {
    bool isActive = false;
    float position[2] = { -100, -100 };
    Texture itemTexture;
    Sprite itemSprite;
    Plant* P;
    Clock timer;
};

class Sunflower : public Plant {
private:
    Sprite sunlight[MAX_SUNLIGHT];
    Clock sunlightTimer;
    Music sunPickup;

public:
    Sunflower(float scaleX = 0.5f, float scaleY = 0.5f) : Plant("Images/img2.png", "sunflower", 200, 0, 100) {
        sprite.setScale(scaleX, scaleY);
        sunPickup.openFromFile("Audios/sunpickup.mp3");
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    void updateSunlight(float x, float y, RenderWindow& window, Texture& sunlightTexture) {
        if (sunlightTimer.getElapsedTime().asSeconds() > 6) {
            sunlightTimer.restart();

            for (int i = 0; i < MAX_SUNLIGHT; i++) {
                if (sunlight[i].getPosition().x == 0) {
                    sunlight[i].setTexture(sunlightTexture);
                    sunlight[i].setPosition(x + 45, y);
                    break;
                }
            }
        }
    }

    void drawSunlight(RenderWindow& window, Texture& sunlightTexture) {
        for (int i = 0; i < MAX_SUNLIGHT; i++) {
            if (sunlight[i].getPosition().x != 0) { // Draw only if sunflower sprite is active
                window.draw(sunlight[i]);
            }
        }
    }

    void moveSunlight() {
        for (int i = 0; i < MAX_SUNLIGHT; i++) {
            if (sunlight[i].getPosition().x != 0) {
                sunlight[i].move(2, -2);
            }
        }
    }

    bool checkSunlightClick(const int mouseX, const int mouseY, int& score) {
        bool sunlightClicked = false;

        // Check if any sunlight was clicked
        for (int i = 0; i < MAX_SUNLIGHT; i++) {
            if (!sunlight[i].getPosition().x == 0) {
                // Check if the mouse click intersects with the sunlight
                if (mouseX >= sunlight[i].getPosition().x && mouseX <= sunlight[i].getPosition().x + 50 &&
                    mouseY >= sunlight[i].getPosition().y && mouseY <= sunlight[i].getPosition().y + 50) {
                    score += 25;
                    sunPickup.play();
                    sunlight[i].setPosition(-100, -100);
                    sunlightClicked = true;
                    break;
                }
            }
        }

        return sunlightClicked;
    }

    Sprite* getSunlight() {
        return sunlight;
    }

    void draw(RenderWindow& window, Texture& sunlightTexture) {
        Plant::draw(window);
        drawSunlight(window, sunlightTexture);
    }

    virtual void abstract() {

    }
};

class Peashooter : public Plant {
private:
    Sprite peas[MAX_PEA];
    Clock peaTimer;

public:
    Peashooter(float scaleX = 0.5f, float scaleY = 0.5f) : Plant("Images/peashooter.png", "peashooter", 200, 33, 100) {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    void drawPeas(RenderWindow& window, Texture& peaTexture) {
        for (int i = 0; i < MAX_PEA; i++) {
            if (peas[i].getPosition().x != 0) { // Draw only if pea sprite is active
                window.draw(peas[i]);
            }
        }
    }

    void updatePeas(float x, float y, RenderWindow& window, Texture& peaTexture) {
        if (peaTimer.getElapsedTime().asSeconds() > 4) { // 4
            peaTimer.restart();

            for (int i = 0; i < MAX_PEA; i++) {
                if (peas[i].getPosition().x == 0) {
                    peas[i].setTexture(peaTexture);
                    peas[i].setPosition(x + 45, y);
                    break;
                }
            }
        }
    }

    void movePeas() {
        for (int i = 0; i < MAX_PEA; i++) {
            if (peas[i].getPosition().x != 0) {
                peas[i].move(12, 0);
            }
        }
    }

    Sprite* getPeas() {
        return peas;
    }

    void draw(RenderWindow& window, Texture& peaTexture) {
        Plant::draw(window);
        drawPeas(window, peaTexture);
    }

    virtual void abstract() {

    }
};

class Wallnut : public Plant {
public:
    Wallnut(float scaleX = 1.0f, float scaleY = 1.0f) : Plant("Images/wall1.png", "wallnut", 300, 500, 200) {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    void draw(RenderWindow& window) {
        Plant::draw(window);
    }

    virtual void abstract() {

    }
};

class Repeater : public Plant {
private:
    Sprite peas[MAX_PEA];
    Clock peaTimer;
    bool doublePeas;

public:
    Repeater(float scaleX = 0.5f, float scaleY = 0.5f) : Plant("Images/repeater.png", "repeater", 200, 20, 200) {
        sprite.setScale(scaleX, scaleY);
        doublePeas = false;
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    void drawPeas(RenderWindow& window, Texture& peaTexture) {
        for (int i = 0; i < MAX_PEA; i++) {
            if (peas[i].getPosition().x != 0) { // Draw only if pea sprite is active
                window.draw(peas[i]);
            }
        }
    }

    void updatePeas(float x, float y, RenderWindow& window, Texture& peaTexture) {
        if (peaTimer.getElapsedTime().asSeconds() > 4) { // 4
            peaTimer.restart();

            for (int i = 0; i < MAX_PEA; i++) {
                if (peas[i].getPosition().x == 0) {
                    peas[i].setTexture(peaTexture);
                    peas[i].setPosition(x + 65, y);
                    break;
                }
            }
            for (int i = 0; i < MAX_PEA; i++) {
                if (peas[i].getPosition().x == 0) {
                    peas[i].setTexture(peaTexture);
                    peas[i].setPosition(x + 35, y);
                    break;
                }
            }
        }
    }

    void movePeas() {
        for (int i = 0; i < MAX_PEA; i++) {
            if (peas[i].getPosition().x != 0) {
                peas[i].move(12, 0);
            }
        }
    }

    Sprite* getPeas() {
        return peas;
    }

    void draw(RenderWindow& window, Texture& peaTexture) {
        Plant::draw(window);
        drawPeas(window, peaTexture);
    }

    virtual void abstract() {

    }
};

class Snow : public Plant {
private:
    Sprite peas[MAX_PEA];
    Clock peaTimer;

public:
    Snow(float scaleX = 0.5f, float scaleY = 0.5f) : Plant("Images/snow.png", "snow", 200, 33, 100) {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    void drawPeas(RenderWindow& window, Texture& peaTexture) {
        for (int i = 0; i < MAX_PEA; i++) {
            if (peas[i].getPosition().x != 0) { // Draw only if pea sprite is active
                window.draw(peas[i]);
            }
        }
    }

    void updatePeas(float x, float y, RenderWindow& window, Texture& peaTexture) {
        if (peaTimer.getElapsedTime().asSeconds() > 3) { // 4
            peaTimer.restart();

            for (int i = 0; i < MAX_PEA; i++) {
                if (peas[i].getPosition().x == 0) {
                    peas[i].setTexture(peaTexture);
                    peas[i].setPosition(x + 45, y);
                    break;
                }
            }
        }
    }

    void movePeas() {
        for (int i = 0; i < MAX_PEA; i++) {
            if (peas[i].getPosition().x != 0) {
                peas[i].move(12, 0);
            }
        }
    }

    Sprite* getPeas() {
        return peas;
    }

    void draw(RenderWindow& window, Texture& peaTexture) {
        Plant::draw(window);
        drawPeas(window, peaTexture);
    }

    virtual void abstract() {

    }
};

class Cherry : public Plant {
private:
    Clock timer;
public:
    Cherry(float scaleX = 1.0f, float scaleY = 1.0f) : Plant("Images/cherry.png", "cherry", 200, 500, 150) {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    void detonate() {
        if (timer.getElapsedTime().asSeconds() > 2) {

        }
    }

    void draw(RenderWindow& window) {
        Plant::draw(window);
    }

    virtual void abstract() {

    }
};