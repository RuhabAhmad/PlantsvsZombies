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

class Score {
private:
    Texture sunTexture;
    Sprite sunSprite;
    Font font;
    Text scoreText;
    RectangleShape background; // Add a background shape

public:
    int scoreValue;
    int lives;
    Score() : scoreValue(50) {
        // Load sun texture
        if (!sunTexture.loadFromFile("sun.png")) {
            cout << "Failed to load sun image!" << endl;
        }
        sunSprite.setTexture(sunTexture);
        sunSprite.setScale(0.5f, 0.5f); // Scale down the sun image if needed
        sunSprite.setPosition(225, 8); // Adjust position as needed

        // Load font for score text
        if (!font.loadFromFile("futura_bold.ttf")) {
            cout << "Failed to load font!" << endl;
        }
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(120, 10); // Adjust position as needed

        // Set up background
        background.setSize(Vector2f(150, 40)); // Set background size to match text area
        background.setFillColor(Color::Black); // Set background color to black
        background.setPosition(108, 8); // Adjust position as needed
    }

    void addScore(int& value) {
        scoreValue += value;
    }

    void subtractScore(int value) {
        scoreValue -= value;
    }

    int getScore() {
        return scoreValue;
    }

    void update() {
        // Update score text
        scoreText.setString("Score: " + to_string(scoreValue));
    }

    void draw(RenderWindow& window) {
        // Draw background
        window.draw(background);
        // Draw sun image and score text
        window.draw(sunSprite);
        window.draw(scoreText);
    }
};
