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

class Level1 : public Level {
private:
    RenderWindow window;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture pauseButtonTexture;
    Sprite pauseButtonSprite;
    bool paused;

public:
    static const int MAX_ZOMBIES = 5;
    SimpleZombie zombies[MAX_ZOMBIES];
    LawnMower mower;
    int numZombies;
    Clock zombieTimer; // Timer to track the time between zombie spawns
    float lastZombiePositionY; // Last spawned zombie's y position
    
    Texture sunflowerTexture;
    Texture peaShooterTexture;
    Sprite sunflowerSprite;
    Sprite peaShooterSprite;

    Level1(RenderWindow& window) : Level("Images/SinglePatch.png"), paused(false) {
        sunflowerTexture.loadFromFile("Images/sunflowercard.png");
        sunflowerSprite.setTexture(sunflowerTexture);
        sunflowerSprite.setScale(1.3f, 1.3f);
        sunflowerSprite.setPosition(5, 2);

        // Load pea shooter texture
        peaShooterTexture.loadFromFile("Images/peacard.png");
        peaShooterSprite.setTexture(peaShooterTexture);
        peaShooterSprite.setScale(1.3f, 1.3f);
        peaShooterSprite.setPosition(5, 120);

        mower.setPosition(200, 2 * 110 + 100);

        numZombies = 0;
        lastZombiePositionY = 0;

        backgroundMusic.play();

        pauseButtonTexture.loadFromFile("Images/button_menus/pause.png");
        pauseButtonSprite.setTexture(pauseButtonTexture);
        pauseButtonSprite.setScale(0.5f, 0.5f);
        pauseButtonSprite.setPosition(1000, 10);
    };

    void handlePause(RenderWindow& window) {
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i mousePosition = Mouse::getPosition(window);
            if (pauseButtonSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                paused = !paused;
                if (paused) {
                    // Pause the background music
                    backgroundMusic.pause();
                }
                else {
                    // Resume the background music
                    backgroundMusic.play();
                }
            }
        }
    }

    void spawnZombie(int x, int y) {
        y = 2 * 110 + 85;
        if (zombieTimer.getElapsedTime().asSeconds() > 20) { // 20 seconds
            zombieTimer.restart();

            if (numZombies < MAX_ZOMBIES) {
                zombies[numZombies].setPosition(x, y);
                numZombies++;
                lastZombiePositionY = y;
            }
        }
    }

    void eatZombies() {
        for (int i = 0; i < numZombies; i++) {
            if (zombies[i].spawned) {
                if (zombies[i].animation.getElapsedTime().asMilliseconds() <= 500) {
                    zombies[i].setTexture("Images/eat1.png");
                }
                else if (zombies[i].animation.getElapsedTime().asMilliseconds() > 500 && zombies[i].animation.getElapsedTime().asMilliseconds() <= 1000) {
                    zombies[i].setTexture("Images/eat2.png");
                }
                else {
                    zombies[i].animation.restart();
                }
            }
        }
    }

    void moveZombies() {
        for (int i = 0; i < numZombies; i++) {
            if (zombies[i].spawned) {
                zombies[i].sprite.move(-10, 0); // Move zombies to the left (-2, 0)
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

    bool moveLawnMower() {
        static bool move = false;

        for (int i = 0; i < numZombies; i++) {
            if (zombies[i].spawned && mower.sprite.getGlobalBounds().intersects(zombies[i].sprite.getGlobalBounds())) {
                move = true;
                break;
            }
        }

        if (move) {
            mower.sprite.move(30, 0);
        }

        return move;
    }

    void drawZombies(RenderWindow& window) {
        for (int i = 0; i < numZombies; i++) {
            if (zombies[i].spawned) {
                zombies[i].draw(window);
            }
        }
    }

    void drawLawnMower(RenderWindow& window) {
        mower.draw(window);
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

    int handleInventoryItemClicks(RenderWindow& window, InventoryItem items[MAX_PLANTS], const int mouseX, const int mouseY, int& score) {
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

    // Collision between zombie and pea
    void handlePeaCollisions(InventoryItem items[MAX_PLANTS], SimpleZombie zombies[], const int maxZombies) {
        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive && items[i].P->type == "peashooter") {
                Peashooter* peashooter = dynamic_cast<Peashooter*>(items[i].P);
                if (peashooter) {
                    Sprite* peas = peashooter->getPeas();
                    for (int j = 0; j < MAX_PEA; j++) {
                        if (peas[j].getPosition().x != 0) { // Check if pea is active
                            for (int k = 0; k < maxZombies; k++) {
                                if (zombies[k].spawned && peas[j].getGlobalBounds().intersects(zombies[k].sprite.getGlobalBounds())) {
                                    zombies[k].health -= peashooter->attack; // Decrease zombie's health
                                    zombies[k].peaCollision.play();
                                    peas[j].setPosition(0, 0); // Remove the pea
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    bool handleItemCollisions(InventoryItem items[MAX_PLANTS], SimpleZombie zombies[], const int maxZombies) {
        bool attacked = false;
        static Clock attackTimer;

        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive) {
                Plant* plant = items[i].P;
                for (int j = 0; j < maxZombies; j++) {
                    if (zombies[j].spawned && items[i].itemSprite.getGlobalBounds().intersects(zombies[j].sprite.getGlobalBounds())) {
                        if (attackTimer.getElapsedTime().asSeconds() > 1) {
                            attackTimer.restart();

                            plant->health -= zombies[j].attack;
                            zombies[j].zombieEat.play();
                        }

                        attacked = true;
                        return attacked;
                    }
                }
            }
        }
        return attacked;
    }

    bool handleZombieDeaths(SimpleZombie zombies[], const int maxZombies) {
        static int count = 0;
        for (int i = 0; i < maxZombies; i++) {
            if (zombies[i].spawned) {
                if (zombies[i].health <= 0) {
                    zombies[i].spawned = false; // Zombie died, so set spawned to false
                    count++;
                }
            }
        }

        // Game loss condition
        if (count >= maxZombies) {
            return false;
        }
        else {
            return true;
        }
    }

    void handleItemDeaths(InventoryItem items[MAX_PLANTS]) {
        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive) {
                Plant* plant = items[i].P;
                if (plant->health <= 0) {
                    items[i].isActive = false;
                    cout << "Plant died." << endl;
                }
            }
        }
    }

    void handleLawnMowerDeaths(LawnMower& mower, SimpleZombie zombies[], const int maxZombies) {
        for (int i = 0; i < maxZombies; i++) {
            if (zombies[i].spawned && mower.sprite.getGlobalBounds().intersects(zombies[i].sprite.getGlobalBounds())) {
                zombies[i].health -= mower.getAttack();
            }
        }
    }

    bool handleGameLoss(SimpleZombie zombies[], const int maxZombies, const int windowWidth) {
        bool flag = false;

        for (int i = 0; i < maxZombies; i++) {
            if (zombies[i].spawned) {
                if (zombies[i].sprite.getPosition().x <= 190) {
                    lives--;
                    if (lives <= 0) {
                        endMusic.play();
                        flag = true;
                    }
                }
            }
        }

        return flag;
    }

    bool isWin() {
		return winTimer.getElapsedTime().asSeconds() >= 2;
	}

    bool isEnd() {
		return endTimer.getElapsedTime().asSeconds() >= 1;
	}

    void handleGameLogic(InventoryItem items[MAX_PLANTS], SimpleZombie zombies[], const int maxZombies) {
        handlePeaCollisions(items, zombies, maxZombies);
        handleItemCollisions(items, zombies, maxZombies);
        handleZombieDeaths(zombies, maxZombies);
        handleItemDeaths(items);
    }

    void run() {
        // Draw background.
        window.draw(backgroundSprite);

        srand(time(NULL));
        RenderWindow window(VideoMode(1200, 700), "PLANT V. ZOMBIES");
        Texture sunlightTexture;
        sunlightTexture.loadFromFile("sun.png");
        Texture sunflowerTexture;
        sunflowerTexture.loadFromFile("Images/img2.png");
        Texture peashooterTexture;
        peashooterTexture.loadFromFile("Images/peashooter.png");
        Texture peaTexture;
        peaTexture.loadFromFile("Images/pea.png");

        Clock temp;
        if (temp.getElapsedTime().asSeconds() > 4) {
            zombiesComing.play();
        }

        Sunlight sunlights[MAX_SUNLIGHTS];
        InventoryItem items[MAX_PLANTS];
        Score score;
        GameState gameState = Idle; // Initialize game state to idle
        Clock delayTimer; // Timer to track the delay Clock

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    handlePause(window); // Check for pause button click
                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    if (gameState == PlacingItem) {
                        // If placing item, place the selected item at mouse coordinates
                        if (selectedItemIndex != -1) {
                            items[selectedItemIndex].position[0] = mouseX - 50;
                            items[selectedItemIndex].position[1] = mouseY - 50;
                            itemPlaced.play();

                            // Reset selected item index and game state
                            selectedItemIndex = -1;
                            gameState = Idle;
                        }
                    }
                    else {
                        int* currentScore = &score.scoreValue;
                        handleSunlightClicks(sunlights, mouseX, mouseY, *currentScore);

                        for (int i = 0; i < MAX_PLANTS; i++) {
                            if (items[i].isActive && items[i].P->type == "sunflower") {
                                Sunflower* sunflower = dynamic_cast<Sunflower*>(items[i].P);
                                if (sunflower) {
                                    sunflower->checkSunlightClick(mouseX, mouseY, *currentScore);
                                }
                            }
                        }

                        selectedItemIndex = handleInventoryItemClicks(window, items, mouseX, mouseY, *currentScore);

                        // If an inventory item is selected, change game state to PlacingItem
                        if (selectedItemIndex != -1) {
                            gameState = PlacingItem;
                        }
                    }
                }
            }

            if (!paused) {
                // spawn handling.
                handleSunlight(sunlights, window.getSize().y);

                updateLives();
                score.update();
                window.clear();

                // backgrounds.
                createBack(window);
                createMap(window);

                drawSunlight(window, sunlights, sunlightTexture);
                drawItem(window, items, selectedItemIndex);

                // Update and draw pea sprites for each Peashooter
                for (int i = 0; i < MAX_PLANTS; i++) {
                    if (items[i].isActive && items[i].P->type == "peashooter") {
                        Peashooter* peashooter = dynamic_cast<Peashooter*>(items[i].P);
                        if (peashooter) {
                            peashooter->updatePeas(items[i].position[0], items[i].position[1], window, peaTexture);
                            peashooter->movePeas();
                            peashooter->draw(window, peaTexture);
                        }
                    }
                }

                // Update and draw sunlight sprites for each Sunflower
                for (int i = 0; i < MAX_PLANTS; i++) {
                    if (items[i].isActive && items[i].P->type == "sunflower") {
                        Sunflower* sunflower = dynamic_cast<Sunflower*>(items[i].P);
                        if (sunflower) {
                            sunflower->updateSunlight(items[i].position[0], items[i].position[1], window, sunlightTexture);
                            sunflower->moveSunlight();
                            sunflower->draw(window, sunlightTexture);
                        }
                    }
                }
                handleGameLogic(items, zombies, MAX_ZOMBIES);

                // Handle collisions between peas and zombies
                handlePeaCollisions(items, zombies, MAX_ZOMBIES);
                bool zombieCollide = handleItemCollisions(items, zombies, MAX_ZOMBIES);

                // zombies.
                spawnZombie(1200, rand() % 550);
                if (zombieCollide) {
                    eatZombies();
                }
                else if (!zombieCollide) {
                    moveZombies();
                }
                drawZombies(window);

                // lawn mower logic
                drawLawnMower(window);
                bool mowerSound = moveLawnMower();
                handleLawnMowerDeaths(mower, zombies, MAX_ZOMBIES);

                if (mowerSound) {
                    static int mowerSoundTemp = 0;
                    if (mowerSoundTemp == 0) {
                        mower.lawnMowerStart.play();
                        mowerSoundTemp++;
                    }
                }

                // Handle deaths
                handleItemDeaths(items);
                if (handleZombieDeaths(zombies, MAX_ZOMBIES)) {
                    winTimer.restart();
                }
                if (!handleGameLoss(zombies, MAX_ZOMBIES, window.getSize().x)) {
                    endTimer.restart();
                }

                // Game win
                if (winTimer.getElapsedTime().asSeconds() >= 2) {
                    static int winSound = 0;
                    if (winSound == 0) { // To only play this sound once
                        winMusic.play();
                        winSound++;
                    }
                    window.close();
                }

                //Game loss
                if (endTimer.getElapsedTime().asSeconds() >= 1) {
                    endMusic.play();
                    window.close();
                }
            }

            drawLives(window);
            window.draw(pauseButtonSprite);
            score.draw(window);
            drawInventory(window);
            window.display();
        }
    }
};

