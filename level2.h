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

class Level2 : public Level {
private:
    RenderWindow window;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture pauseButtonTexture;
    Sprite pauseButtonSprite;
    bool paused;

public:
    static const int MAX_ZOMBIES = 5;
    SimpleZombie zombies1[MAX_ZOMBIES];
    FootballZombie zombies2[MAX_ZOMBIES];
    ConeZombie zombies3[MAX_ZOMBIES];
    LawnMower mower[3];
    int numZombies1;
    int numZombies2;
    int numZombies3;
    Clock zombieTimer; // Timer to track the time between zombie spawns
    float lastZombiePositionY1; // Last spawned zombie's y position
    float lastZombiePositionY2;
    float lastZombiePositionY3;

    Level2(RenderWindow& window) : Level("Images/level2.jpg"), paused(false) {
        // Load sunflower card
        sunflowerCardTexture.loadFromFile("Images/sunflowercard.png");
        sunflowerCardSprite.setTexture(sunflowerCardTexture);
        sunflowerCardSprite.setScale(1.3f, 1.3f);
        sunflowerCardSprite.setPosition(5, 2);

        // Load peashooter card
        peaShooterCardTexture.loadFromFile("Images/peacard.png");
        peaShooterCardSprite.setTexture(peaShooterCardTexture);
        peaShooterCardSprite.setScale(1.3f, 1.3f);
        peaShooterCardSprite.setPosition(5, 120);

        // Load repeater card
        repeaterCardTexture.loadFromFile("Images/repeatercard.png");
        repeaterCardSprite.setTexture(repeaterCardTexture);
        repeaterCardSprite.setScale(1.3f, 1.3f);
        repeaterCardSprite.setPosition(5, 235);

        // Load wallnut card
        wallnutCardTexture.loadFromFile("Images/wallnutcard.png");
        wallnutCardSprite.setTexture(wallnutCardTexture);
        wallnutCardSprite.setScale(1.3f, 1.3f);
        wallnutCardSprite.setPosition(5, 350);

        for (int i = 0; i < 3; i++) {
            int y = i * 110 + 225;
            if (i == 2) {
                y += 10;
            }
            mower[i].setPosition(200, y);
        }

        numZombies1 = 0;
        numZombies2 = 0;
        numZombies3 = 0;
        lastZombiePositionY1 = 0;
        lastZombiePositionY2 = 0;
        lastZombiePositionY3 = 0;

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
        y = (rand() % 3) * 120 + 200;
        if (zombieTimer.getElapsedTime().asSeconds() > 16) { // 20 seconds
            zombieTimer.restart();

            int temp = rand() % 3;

            if (temp == 0) {
                if (numZombies1 < MAX_ZOMBIES) {
                    zombies1[numZombies1].setScale(0.9, 0.9);
                    zombies1[numZombies1].setPosition(x, y);
                    zombies1[numZombies1].spawned = true;
                    numZombies1++;
                    lastZombiePositionY1 = y;
                    cout << temp << endl;
                }
            }
            else if (temp == 1) {
                if (numZombies2 < MAX_ZOMBIES) {
                    zombies2[numZombies2].setPosition(x, y);
                    zombies2[numZombies2].spawned = true;
                    numZombies2++;
                    lastZombiePositionY2 = y;
                    cout << temp << endl;
                }
            }
            else if (temp == 2) {
                if (numZombies3 < MAX_ZOMBIES) {
                    zombies3[numZombies3].setPosition(x, y - 20);
                    zombies3[numZombies3].spawned = true;
                    numZombies3++;
                    lastZombiePositionY3 = y;
                    cout << temp << endl;
                }
            }
        }
    }

    void eatZombies1() {
        for (int i = 0; i < numZombies1; i++) {
            if (zombies1[i].spawned) {
                zombies1[i].eat();
            }
        }
    }
    void eatZombies2() {
        for (int i = 0; i < numZombies2; i++) {
            if (zombies2[i].spawned) {
                zombies2[i].eat();
            }
        }
    }
    void eatZombies3() {
        for (int i = 0; i < numZombies3; i++) {
            if (zombies3[i].spawned) {
                zombies3[i].eat();
            }
        }
    }

    void moveZombies1() {
        for (int i = 0; i < numZombies1; i++) {
            if (zombies1[i].spawned) {
                zombies1[i].moveZombie();
            }
        }
    }
    void moveZombies2() {
        for (int i = 0; i < numZombies2; i++) {
            if (zombies2[i].spawned) {
                zombies2[i].moveZombie();
            }
        }
    }
    void moveZombies3() {
        for (int i = 0; i < numZombies3; i++) {
            if (zombies3[i].spawned) {
                zombies3[i].moveZombie();
            }
        }
    }

    void moveLawnMower1() {
        static bool move = false;
        static int t = 0;
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            for (int j = 0; j < 3; j++) {
                if (mower[j].sprite.getGlobalBounds().intersects(zombies1[i].sprite.getGlobalBounds())) {
                    zombies1[i].sprite.setPosition(-1000, -1000);
                    move = true;
                    t = j;
                    break;
                }
            }
        }
        if (move) {
            mower[t].sprite.move(15, 0);
        }
    }
    void moveLawnMower2() {
        static bool move = false;
        static int t = 0;
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            for (int j = 0; j < 3; j++) {
                if (mower[j].sprite.getGlobalBounds().intersects(zombies2[i].sprite.getGlobalBounds())) {
                    zombies2[i].sprite.setPosition(-1000, -1000);
                    move = true;
                    t = j;
                    break;
                }
            }
        }
        if (move) {
            mower[t].sprite.move(15, 0);
        }
    }
    void moveLawnMower3() {
        static bool move = false;
        static int t = 0;
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            for (int j = 0; j < 3; j++) {
                if (mower[j].sprite.getGlobalBounds().intersects(zombies3[i].sprite.getGlobalBounds())) {
                    zombies3[i].sprite.setPosition(-1000, -1000);
                    move = true;
                    t = j;
                    break;
                }
            }
        }
        if (move) {
            mower[t].sprite.move(15, 0);
        }
    }

    void drawZombies(RenderWindow& window) {
        for (int i = 0; i < numZombies1; i++) {
            if (zombies1[i].spawned) {
                zombies1[i].draw(window);
            }
        }
        for (int i = 0; i < numZombies2; i++) {
            if (zombies2[i].spawned) {
                zombies2[i].draw(window);
            }
        }
        for (int i = 0; i < numZombies3; i++) {
            if (zombies3[i].spawned) {
                zombies3[i].draw(window);
            }
        }
    }

    void drawLawnMower(RenderWindow& window) {
        for (int i = 0; i < 3; i++) {
            mower[i].draw(window);
        }
    }

    void placePlant(Plant& plant, int x, int y, RenderWindow& window) {
        plant.setPosition(x, y);
        plant.draw(window);
    }

    void drawInventory(RenderWindow& window) {
        window.draw(sunflowerCardSprite);
        window.draw(peaShooterCardSprite);
        window.draw(repeaterCardSprite);
        window.draw(wallnutCardSprite);
    }

    void drawItem(RenderWindow& window, InventoryItem items[MAX_PLANTS], int itemIndex) {
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
        // Check if repeater card was clicked
        else if (mouseY >= 235 && mouseY <= 330 && mouseX >= 5 && mouseX <= 88 && score >= 200) {
            if (itemIndex < MAX_PLANTS) {
                if (!items[itemIndex].isActive) {
                    items[itemIndex].isActive = true;
                    items[itemIndex].itemTexture = repeaterTexture;
                    items[itemIndex].itemSprite.setTexture(repeaterTexture);
                    items[itemIndex].P = new Repeater;
                    items[itemIndex].P->setHealth(100);
                    items[itemIndex].P->setAttack(20);
                    cout << "REPEATER CLICKED" << endl;

                    itemIndex++;
                    score -= 200;
                }
                itemClicked = true;

                return itemIndex - 1;
            }
        }
        // Check if wallnut card was clicked
        else if (mouseY >= 350 && mouseY <= 445 && mouseX >= 5 && mouseX <= 88 && score >= 50) {
            if (itemIndex < MAX_PLANTS) {
                if (!items[itemIndex].isActive) {
                    items[itemIndex].isActive = true;
                    items[itemIndex].itemTexture = wallnutTexture;
                    items[itemIndex].itemSprite.setTexture(wallnutTexture);
                    items[itemIndex].P = new Wallnut;
                    items[itemIndex].P->setHealth(300);
                    items[itemIndex].P->setAttack(500);
                    cout << "WALLNUT CLICKED" << endl;

                    itemIndex++;
                    score -= 50;
                }
                itemClicked = true;

                return itemIndex - 1;
            }
        }

        return -1;
    }

    // Collision between zombie and pea
    void handlePeaCollisions(InventoryItem items[MAX_PLANTS]) {
        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive) {
                if (items[i].P->type == "peashooter") {
                    Peashooter* peashooter = dynamic_cast<Peashooter*>(items[i].P);
                    if (peashooter) {
                        Sprite* peas = peashooter->getPeas();
                        for (int j = 0; j < MAX_PEA; j++) {
                            if (peas[j].getPosition().x != 0) { // Check if pea is active
                                for (int k = 0; k < MAX_ZOMBIES; k++) {
                                    if (zombies1[k].spawned && peas[j].getGlobalBounds().intersects(zombies1[k].sprite.getGlobalBounds())) {
                                        zombies1[k].health -= peashooter->attack; // Decrease zombie's health
                                        zombies1[k].peaCollision.play();
                                        peas[j].setPosition(0, 0); // Remove the pea
                                    }
                                }
                                for (int k = 0; k < MAX_ZOMBIES; k++) {
                                    if (zombies2[k].spawned && peas[j].getGlobalBounds().intersects(zombies2[k].sprite.getGlobalBounds())) {
                                        zombies2[k].health -= peashooter->attack; // Decrease zombie's health
                                        zombies2[k].peaCollision.play();
                                        peas[j].setPosition(0, 0); // Remove the pea
                                    }
                                }
                                for (int k = 0; k < MAX_ZOMBIES; k++) {
                                    if (zombies3[k].spawned && peas[j].getGlobalBounds().intersects(zombies3[k].sprite.getGlobalBounds())) {
                                        zombies3[k].health -= peashooter->attack; // Decrease zombie's health
                                        zombies3[k].peaCollision.play();
                                        peas[j].setPosition(0, 0); // Remove the pea
                                    }
                                }

                            }
                        }
                    }
                }
                else if (items[i].P->type == "repeater") {
                    Repeater* repeater = dynamic_cast<Repeater*>(items[i].P);
                    if (repeater) {
                        Sprite* peas = repeater->getPeas();
                        for (int j = 0; j < MAX_PEA; j++) {
                            if (peas[j].getPosition().x != 0) { // Check if pea is active
                                for (int k = 0; k < MAX_ZOMBIES; k++) {
                                    if (zombies1[k].spawned && peas[j].getGlobalBounds().intersects(zombies1[k].sprite.getGlobalBounds())) {
                                        zombies1[k].health -= repeater->attack; // Decrease zombie's health
                                        zombies1[k].peaCollision.play();
                                        peas[j].setPosition(0, 0); // Remove the pea
                                    }
                                }
                                for (int k = 0; k < MAX_ZOMBIES; k++) {
                                    if (zombies2[k].spawned && peas[j].getGlobalBounds().intersects(zombies2[k].sprite.getGlobalBounds())) {
                                        zombies2[k].health -= repeater->attack; // Decrease zombie's health
                                        zombies2[k].peaCollision.play();
                                        peas[j].setPosition(0, 0); // Remove the pea
                                    }
                                }
                                for (int k = 0; k < MAX_ZOMBIES; k++) {
                                    if (zombies3[k].spawned && peas[j].getGlobalBounds().intersects(zombies3[k].sprite.getGlobalBounds())) {
                                        zombies3[k].health -= repeater->attack; // Decrease zombie's health
                                        zombies3[k].peaCollision.play();
                                        peas[j].setPosition(0, 0); // Remove the pea
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    bool handleItemCollision1(InventoryItem items[MAX_PLANTS]) {
        bool attacked = false;
        static Clock attackTimer;

        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive) {
                Plant* plant = items[i].P;
                for (int j = 0; j < MAX_ZOMBIES; j++) {
                    if (zombies1[j].spawned && items[i].itemSprite.getGlobalBounds().intersects(zombies1[j].sprite.getGlobalBounds())) {
                        if (attackTimer.getElapsedTime().asSeconds() > 1) {
                            attackTimer.restart();

                            plant->health -= zombies1[j].attack;
                            zombies1[j].zombieEat.play();
                        }

                        attacked = true;
                        return attacked;
                    }
                }
            }
        }
        return attacked;
    }
    bool handleItemCollision2(InventoryItem items[MAX_PLANTS]) {
        bool attacked = false;
        static Clock attackTimer;

        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive) {
                Plant* plant = items[i].P;
                for (int j = 0; j < MAX_ZOMBIES; j++) {
                    if (zombies2[j].spawned && items[i].itemSprite.getGlobalBounds().intersects(zombies2[j].sprite.getGlobalBounds())) {
                        if (attackTimer.getElapsedTime().asSeconds() > 1) {
                            attackTimer.restart();

                            plant->health -= zombies2[j].attack;
                            zombies1[j].zombieEat.play();
                        }

                        attacked = true;
                        return attacked;
                    }
                }
            }
        }
        return attacked;
    }
    bool handleItemCollision3(InventoryItem items[MAX_PLANTS]) {
        bool attacked = false;
        static Clock attackTimer;

        for (int i = 0; i < MAX_PLANTS; i++) {
            if (items[i].isActive) {
                Plant* plant = items[i].P;
                for (int j = 0; j < MAX_ZOMBIES; j++) {
                    if (zombies3[j].spawned && items[i].itemSprite.getGlobalBounds().intersects(zombies3[j].sprite.getGlobalBounds())) {
                        if (attackTimer.getElapsedTime().asSeconds() > 1) {
                            attackTimer.restart();

                            plant->health -= zombies3[j].attack;
                            zombies1[j].zombieEat.play();
                        }

                        attacked = true;
                        return attacked;
                    }
                }
            }
        }
        return attacked;
    }

    bool handleZombieDeaths() {
        static int count = 0;
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies1[i].spawned) {
                if (zombies1[i].health <= 0) {
                    zombies1[i].spawned = false; // Zombie died, so set spawned to false
                    count++;
                }
            }
        }
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies2[i].spawned) {
                if (zombies2[i].health <= 0) {
                    zombies2[i].spawned = false; // Zombie died, so set spawned to false
                    count++;
                }
            }
        }
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies3[i].spawned) {
                if (zombies3[i].health <= 0) {
                    zombies3[i].spawned = false; // Zombie died, so set spawned to false
                    count++;
                }
            }
        }

        // Game win condition
        if (count >= MAX_ZOMBIES * 3) {
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

    void handleLawnMowerDeaths() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < MAX_ZOMBIES; j++) {
                if (zombies1[j].spawned && mower[i].sprite.getGlobalBounds().intersects(zombies1[j].sprite.getGlobalBounds())) {
                    zombies1[j].health -= mower[i].getAttack();
                }
            }
            for (int j = 0; j < MAX_ZOMBIES; j++) {
                if (zombies2[j].spawned && mower[i].sprite.getGlobalBounds().intersects(zombies2[j].sprite.getGlobalBounds())) {
                    zombies2[j].health -= mower[i].getAttack();
                }
            }
            for (int j = 0; j < MAX_ZOMBIES; j++) {
                if (zombies3[j].spawned && mower[i].sprite.getGlobalBounds().intersects(zombies3[j].sprite.getGlobalBounds())) {
                    zombies3[j].health -= mower[i].getAttack();
                }
            }
        }
    }

    bool handleGameLoss() {
        bool flag = false;

        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies1[i].spawned) {
                if (zombies1[i].sprite.getPosition().x <= 190) {
                    lives--;
                    if (lives <= 0) {
                        endMusic.play();
                        flag = true;
                    }
                }
            }
        }
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies2[i].spawned) {
                if (zombies2[i].sprite.getPosition().x <= 190) {
                    lives--;
                    if (lives <= 0) {
                        endMusic.play();
                        flag = true;
                    }
                }
            }
        }
        for (int i = 0; i < MAX_ZOMBIES; i++) {
            if (zombies3[i].spawned) {
                if (zombies3[i].sprite.getPosition().x <= 190) {
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

    void handleGameLogic(InventoryItem items[MAX_PLANTS]) {
        handlePeaCollisions(items);
        handleItemCollision1(items);
        handleItemCollision2(items);
        handleItemCollision3(items);
        handleZombieDeaths();
        handleItemDeaths(items);
    }

    bool isWin() {
        return winTimer.getElapsedTime().asSeconds() >= 2;
    }

    bool isEnd() {
        return endTimer.getElapsedTime().asSeconds() >= 1;
    }

    void run() {
        // Draw background.
        window.draw(backgroundSprite);

        srand(time(NULL));
        RenderWindow window(VideoMode(1200, 700), "-GTA VI-");

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

            if (!paused)
            {
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
                    else if (items[i].isActive && items[i].P->type == "repeater") {
                        Repeater* repeater = dynamic_cast<Repeater*>(items[i].P);
                        if (repeater) {
                            repeater->updatePeas(items[i].position[0], items[i].position[1], window, peaTexture);
                            repeater->movePeas();
                            repeater->draw(window, peaTexture);
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

                handleGameLogic(items);

                // Handle collisions between peas and zombies
                handlePeaCollisions(items);

                // zombies.
                spawnZombie(1200, rand() % 550);
                bool zombieCollide1 = handleItemCollision1(items);
                bool zombieCollide2 = handleItemCollision2(items);
                bool zombieCollide3 = handleItemCollision3(items);

                // zombies.
                spawnZombie(1200, rand() % 550);
                if (zombieCollide1) {
                    eatZombies1();
                }
                else if (!zombieCollide1) {
                    moveZombies1();
                }
                if (zombieCollide2) {
                    eatZombies2();
                }
                else if (!zombieCollide2) {
                    moveZombies2();
                }
                if (zombieCollide3) {
                    eatZombies3();
                }
                else if (!zombieCollide3) {
                    moveZombies3();
                }
                drawZombies(window);

                // lawn mower logic
                handleLawnMowerDeaths();
                drawLawnMower(window);
                moveLawnMower1();
                moveLawnMower2();
                moveLawnMower3();

                // Handle deaths
                handleItemDeaths(items);
                if (handleZombieDeaths()) {
                    winTimer.restart();
                }
                if (!handleGameLoss()) {
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
