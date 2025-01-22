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

class Level {
private:
    string temp;

public:
    Texture mapTexture;
    Sprite mapSprite;

    Level(string in) : temp(in) {}

    void createMap(RenderWindow& window) {
        // Load the map image
        Image mapImage;
        if (!mapImage.loadFromFile(temp)) {
            cout << "Failed to load map image!" << endl;
            return;
        }

        // Create texture and sprite
        Texture mapTexture;
        mapTexture.loadFromImage(mapImage);
        Sprite mapSprite(mapTexture);

        // Scale the map sprite to fit the window size
        float scaleX = static_cast<float>(window.getSize().x) / mapImage.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / mapImage.getSize().y;
        mapSprite.setScale(scaleX, scaleY);

        // Draw the map sprite
        window.draw(mapSprite);
    }
};

class Level1 : public Level {
public:
    static const int MAX_ZOMBIES = 5;
    SimpleZombie zombies[MAX_ZOMBIES];
    int numZombies;
    Clock zombieTimer; // Timer to track the time between zombie spawns
    float lastZombiePositionY; // Last spawned zombie's y position

public:
    Texture sunflowerTexture;
    Texture peaShooterTexture;
    Sprite sunflowerSprite;
    Sprite peaShooterSprite;

    Level1(RenderWindow& window) : Level("Images/SinglePatch.png") {
        if (!sunflowerTexture.loadFromFile("Images/sunflowercard.png")) {
            cout << "Failed to load sunflower image!" << endl;
            return;
        }
        sunflowerSprite.setTexture(sunflowerTexture);
        sunflowerSprite.setScale(1.3f, 1.3f);
        sunflowerSprite.setPosition(5, 2);

        // Load pea shooter texture
        if (!peaShooterTexture.loadFromFile("Images/peacard.png")) {
            cout << "Failed to load peashooter image!" << endl;
            return;
        }
        peaShooterSprite.setTexture(peaShooterTexture);
        peaShooterSprite.setScale(1.3f, 1.3f);
        peaShooterSprite.setPosition(5, 120);


        numZombies = 0;
        lastZombiePositionY = 0;
    };

    void spawnZombie(int x, int y) {
        y = 2 * 110 + 85;
        if (zombieTimer.getElapsedTime().asSeconds() > 22) {
            zombieTimer.restart();

            if (numZombies < MAX_ZOMBIES) {
                zombies[numZombies].setPosition(x, y);
                numZombies++;
                lastZombiePositionY = y;
            }
        }
    }

    void moveZombies() {
        for (int i = 0; i < numZombies; i++) {
            if (zombies[i].spawned) {
                zombies[i].sprite.move(-2, 0); // Move zombies to the left
                int xPos = static_cast<int>(zombies[i].sprite.getPosition().x);
                if (xPos % 20 >= 0 && xPos % 20 < 4) {
                    zombies[i].setTexture("Images/Frames/zombie0.png");
                }
                else if (xPos % 20 >= 4 && xPos % 20 < 8) {
                    zombies[i].setTexture("Images/Frames/zombie1.png");
                }
                else if (xPos % 20 >= 8 && xPos % 20 < 12) {
                    zombies[i].setTexture("Images/Frames/zombie2.png");
                }
                else if (xPos % 20 >= 12 && xPos % 20 < 16) {
                    zombies[i].setTexture("Images/Frames/zombie3.png");
                }
                else {
                    zombies[i].setTexture("Images/Frames/zombie4.png");
                }
            }
        }
    }

    void drawZombies(RenderWindow& window) {
        for (int i = 0; i < numZombies; i++) {
            if (zombies[i].spawned) {
                zombies[i].draw(window);
            }
        }
    }

    void placePlant(Plant& plant, int x, int y, RenderWindow& window) {
        plant.setPosition(x, y);
        plant.draw(window);
    }

    void drawInventory(RenderWindow& window) {
        window.draw(sunflowerSprite);
        window.draw(peaShooterSprite);
    }

    void drawItem(RenderWindow& window, InventoryItem items[MAX_PLANTS], int itemIndex) {
        Texture peaTexture;
        peaTexture.loadFromFile("Images/pea.png");

        static Clock peaTimer;
        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive) {
                int x = items[i].position[0];
                int y = items[i].position[1];

                items[i].itemSprite.setPosition(x, y);
                window.draw(items[i].itemSprite);
            }
        }
    }

    int handleInventoryItemClicks(RenderWindow& window, InventoryItem items[MAX_PLANTS], const int mouseX, const int mouseY, int& score, bool x, Level1& level, Texture& sunflowerTexture, Texture& peashooterTexture) {
        bool itemClicked = false;
        static int itemIndex = 0;

        // Check if sunflower card was clicked
        if (mouseY >= 2 && mouseY <= 60 && mouseX >= 5 && mouseX <= 88 && score >= 50) {
            if (itemIndex < MAX_PLANTS) {
                if (!items[itemIndex].isActive) {
                    items[itemIndex].isActive = true;
                    items[itemIndex].itemTexture = sunflowerTexture;
                    items[itemIndex].itemSprite.setTexture(sunflowerTexture);
                    items[itemIndex].P = new Sunflower;
                    items[itemIndex].P->setHealth(100);
                    items[itemIndex].P->setAttack(0);
                    cout << "SUNFLOWER CLICKED" << endl;

                    itemIndex++;
                    score -= 50;
                }

                itemClicked = true;

                return itemIndex - 1;
            }
        }
        // Check if pea shooter card was clicked
        else if (mouseY >= 120 && mouseY <= 178 && mouseX >= 5 && mouseX <= 88 && score >= 100) {
            if (itemIndex < MAX_PLANTS) {
                if (!items[itemIndex].isActive) {
                    items[itemIndex].isActive = true;
                    items[itemIndex].itemTexture = peashooterTexture;
                    items[itemIndex].itemSprite.setTexture(peashooterTexture);
                    items[itemIndex].P = new Peashooter;
                    items[itemIndex].P->setHealth(100);
                    items[itemIndex].P->setAttack(20);
                    cout << "PEASHOOTER CLICKED" << endl;

                    itemIndex++;
                    score -= 100;
                }
                itemClicked = true;

                return itemIndex - 1;
            }
        }

        return -1;
    }
};
