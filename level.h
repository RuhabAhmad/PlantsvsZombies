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

struct Sunlight {
    bool isActive = false; // Indicates if sunlight is active
    float position[2] = { 0, 0 }; // Array to store positions of sunlights
    Clock timer; // Timer for sunlight interval
};

class Level {
private:
    string temp;

public:
    Texture mapTexture;
    Sprite mapSprite;
    Texture selectedItemTexture;
    int selectedItemIndex = -1;
    Clock endTimer;
    Clock winTimer;
    int lives;

    // Textures
    Texture sunflowerCardTexture;
    Texture peaShooterCardTexture;
    Texture wallnutCardTexture;
    Texture repeaterCardTexture;
    Texture snowCardTexture;
    Texture cherryCardTexture;
    Sprite sunflowerCardSprite;
    Sprite peaShooterCardSprite;
    Sprite wallnutCardSprite;
    Sprite repeaterCardSprite;
    Sprite snowCardSprite;
    Sprite cherryCardSprite;

    Texture sunlightTexture;
    Texture sunflowerTexture;
    Texture peashooterTexture;
    Texture repeaterTexture;
    Texture wallnutTexture;
    Texture snowTexture;
    Texture peaTexture;
    Texture snowpeaTexture;
    Texture cherryTexture;

    enum GameState {
        Idle,
        PlacingItem
    };

    GameState gameState = Idle;

    // Audios
    Music backgroundMusic;
    Music zombiesComing;
    Music sunPickup;
    Music itemPlaced;
    Music winMusic;
    Music endMusic;

    Text livesText;
    RectangleShape background;
    Font font;

    Level(string in) : temp(in), lives(1) {
        sunlightTexture.loadFromFile("sun.png");
        sunflowerTexture.loadFromFile("Images/img2.png");
        peashooterTexture.loadFromFile("Images/peashooter.png");
        peaTexture.loadFromFile("Images/pea.png");
        repeaterTexture.loadFromFile("Images/repeater.png");
        wallnutTexture.loadFromFile("Images/wall1.png");
        snowTexture.loadFromFile("Images/snow.png");
        snowpeaTexture.loadFromFile("Images/spea.png");
        cherryTexture.loadFromFile("Images/cherry.png");

        backgroundMusic.openFromFile("Audios/background.mp3");
        zombiesComing.openFromFile("Audios/zombiecoming.mp3");
        sunPickup.openFromFile("Audios/sunpickup.mp3");
        itemPlaced.openFromFile("Audios/plantingsound.mp3");
        endMusic.openFromFile("Audios/lost.mp3");
        winMusic.openFromFile("Audios/win.mp3");

        // Load font for score text
        if (!font.loadFromFile("futura_bold.ttf")) {
            cout << "Failed to load font!" << endl;
        }
        livesText.setFont(font);
        livesText.setCharacterSize(24);
        livesText.setFillColor(Color::White);
        livesText.setPosition(270, 10); // Adjust position as needed

        // Set up background
        background.setSize(Vector2f(150, 40)); // Set background size to match text area
        background.setFillColor(Color::Black); // Set background color to black
        background.setPosition(258, 8); // Adjust position as needed
    }

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

    void createBack(RenderWindow& window) {
        // Load background image
        Image backgroundImage;
        if (!backgroundImage.loadFromFile("backwindow.jpg")) {
            cout << "Failed to load background image!" << endl;
            return;
        }

        // Create texture and sprite
        Texture backgroundTexture;
        backgroundTexture.loadFromImage(backgroundImage);
        Sprite backgroundSprite(backgroundTexture);

        // Scale the background image to fit the window size
        float scaleX = static_cast<float>(window.getSize().x) / backgroundImage.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / backgroundImage.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);

        // Draw the background
        window.draw(backgroundSprite);
    }

    void drawSunlight(RenderWindow& window, Sunlight sunlights[MAX_SUNLIGHTS], Texture& sunlightTexture) {
        Sprite sunlightSprite(sunlightTexture);

        for (int i = 0; i < MAX_SUNLIGHTS; i++) {
            if (sunlights[i].isActive) {
                int x = sunlights[i].position[0]; // Accessing x-coordinate
                int y = sunlights[i].position[1]; // Accessing y-coordinate
                sunlightSprite.setPosition(x, y);

                // Draw sunlight sprite if x is not zero
                if (x >= 100 && x <= 1150) {
                    window.draw(sunlightSprite);
                }

            }
        }
    }

    void handleSunlight(Sunlight sunlights[MAX_SUNLIGHTS], const int windowHeight) {
        static Clock sunlightTimer;

        // Random sunlight instances
        if (sunlightTimer.getElapsedTime().asSeconds() > 6) {
            // Reset timer
            sunlightTimer.restart();

            // Find an inactive sunlight and activate it
            for (int i = 0; i < MAX_SUNLIGHTS; i++) {
                if (!sunlights[i].isActive) {
                    // Activate sunlight
                    sunlights[i].isActive = true;

                    // Set initial position
                    int x = 0;
                    do {
                        x = (rand() % 800) + 300;
                    } while (x < 200);
                    sunlights[i].position[0] = x;
                    sunlights[i].position[1] = 0;

                    break;
                }
            }
        }

        // Move active sunlights
        for (int i = 0; i < MAX_SUNLIGHTS; i++) {
            if (sunlights[i].isActive) {
                // Move sunlight downwards
                sunlights[i].position[1] += 2;

                // Check if sunlight reached the bottom of the screen
                if (sunlights[i].position[1] > windowHeight) {
                    sunlights[i].isActive = false;
                }
            }
        }
    }

    bool handleSunlightClicks(Sunlight sunlights[MAX_SUNLIGHTS], const int mouseX, const int mouseY, int& score) {
        bool sunlightClicked = false;

        // Check if any sunlight was clicked
        for (int i = 0; i < MAX_SUNLIGHTS; i++) {
            if (sunlights[i].isActive) {
                // Check if the mouse click intersects with the sunlight
                if (mouseX >= sunlights[i].position[0] && mouseX <= sunlights[i].position[0] + 50 &&
                    mouseY >= sunlights[i].position[1] && mouseY <= sunlights[i].position[1] + 50) {
                    sunlights[i].isActive = false;
                    sunPickup.play();
                    score += 25;
                    sunlightClicked = true;
                    break;
                }
            }
        }

        return sunlightClicked;
    }

    void updateLives() {
        // Update score text
        if (lives >= 0) {
            livesText.setString("Lives: " + to_string(lives));
        }
    }

    void drawLives(RenderWindow& window) {
        // Draw background
        window.draw(background);
        window.draw(livesText);
    }
};
