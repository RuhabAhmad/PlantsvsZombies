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

class MenuBase {
public:
    MenuBase(RenderWindow& window) : window(window) {}

    virtual void draw() = 0;

protected:
    RenderWindow& window;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture buttonTexture;
    Font font;
};

// Forward declaration of MainMenu class
class MainMenu;

class Button {
public:
    Button(Texture& texture, Font& font, const string& text, Vector2f position)
        : texture(texture), font(font), buttonText(text, font, 24) {
        sprite.setTexture(texture);
        sprite.setPosition(position);

        buttonText.setFillColor(Color::White);
        buttonText.setPosition(position.x + 35, position.y + 10);
    }

    virtual ~Button() {}

    virtual void draw(RenderWindow& window) {
        window.draw(sprite);
        window.draw(buttonText);
    }

    bool contains(Vector2f point) {
        return sprite.getGlobalBounds().contains(point);
    }

protected:
    Texture& texture;
    Font& font;
    Text buttonText;
    Sprite sprite;
};

class ExitButton : public Button {
public:
    ExitButton(Texture& texture, Font& font, const string& text, Vector2f position)
        : Button(texture, font, text, position) {}

    void draw(RenderWindow& window) override {
        Button::draw(window);
    }
};

class PlayButton : public Button {
public:
    PlayButton(Texture& texture, Font& font, const string& text, Vector2f position)
        : Button(texture, font, text, position) {}

    void draw(RenderWindow& window) override {
        Button::draw(window);
    }
};

class InstructionsButton : public Button {
public:
    InstructionsButton(Texture& texture, Font& font, const string& text, Vector2f position)
        : Button(texture, font, text, position) {}

    void draw(RenderWindow& window) override {
        Button::draw(window);
    }
};

class MusicOffButton : public Button {
private:
    bool& musicOn;
    Music& menuMusic;

public:
    MusicOffButton(Texture& texture, Font& font, const string& text, Vector2f position, bool& musicOn, Music& menuMusic)
        : Button(texture, font, text, position), musicOn(musicOn), menuMusic(menuMusic) {}

    void draw(RenderWindow& window) override {
        Button::draw(window);
    }
};

class HighScoreButton : public Button {
public:
    HighScoreButton(Texture& texture, Font& font, const string& text, Vector2f position)
        : Button(texture, font, text, position) {}

    void draw(RenderWindow& window) override {
        Button::draw(window);
    }
};

class MainMenu : public MenuBase {
private:
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture buttonTexture;
    Font font;
    Button* playButton1;
    Button* playButton2;
    Button* playButton3;
    Button* instructionsButton;
    Button* exitButton;
    Button* returnToMenuButton;
    Button* highScoreButton;
    bool showInstructions;
    Music menuMusic; // Declaration of menuMusic
    MusicOffButton* musicOffButton;
    bool musicOn;
public:

    MainMenu(RenderWindow& window) : MenuBase(window), showInstructions(false), musicOn(true) {
        if (!backgroundTexture.loadFromFile("Images/button_menus/mainmenu-01.png")) {
            cout << "Failed to load background image!" << endl;
            window.close();
        }
        backgroundSprite.setTexture(backgroundTexture);
        float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);

        if (!buttonTexture.loadFromFile("Images/button_menus/Button2.png")) {
            cout << "Failed to load button image!" << endl;
            window.close();
        }

        if (!font.loadFromFile("futura_bold.ttf")) {
            cout << "Failed to load font!" << endl;
            window.close();
        }

        playButton1 = new PlayButton(buttonTexture, font, "PLAY", Vector2f(50, 320));
        playButton2 = new PlayButton(buttonTexture, font, "LEVEL 2", Vector2f(50, 370));
        playButton3 = new PlayButton(buttonTexture, font, "LEVEL 3", Vector2f(50, 420));
        instructionsButton = new InstructionsButton(buttonTexture, font, "INSTRUCTIONS", Vector2f(50, 470));
        exitButton = new ExitButton(buttonTexture, font, "EXIT", Vector2f(50, 620));
        //returnToMenuButton = new Button(buttonTexture, font, "RETURN TO MAIN MENU", Vector2f(50, 600));
        musicOffButton = new MusicOffButton(buttonTexture, font, "MUSIC OFF", Vector2f(50, 570), musicOn, menuMusic);
        highScoreButton = new HighScoreButton(buttonTexture, font, "HIGH SCORES", Vector2f(50, 520));
    }

    ~MainMenu() {
        delete playButton1;
        delete playButton2;
        delete playButton3;
        delete instructionsButton;
        delete exitButton;
        delete returnToMenuButton;
        delete musicOffButton;
        delete highScoreButton;
    }

    void draw() override {
        window.draw(backgroundSprite);
        playButton1->draw(window);
        playButton2->draw(window);
        playButton3->draw(window);
        highScoreButton->draw(window);
        instructionsButton->draw(window);
        exitButton->draw(window);
        if (showInstructions) {
            returnToMenuButton->draw(window);
        }
        musicOffButton->draw(window);
    }

    bool isPlay1Selected(Vector2f mousePos) {
        return playButton1->contains(mousePos);
    }

    bool isPlay2Selected(Vector2f mousePos) {
        return playButton2->contains(mousePos);
    }

    bool isPlay3Selected(Vector2f mousePos) {
        return playButton3->contains(mousePos);
    }

    bool isInstructionsSelected(Vector2f mousePos) {
        return instructionsButton->contains(mousePos);
    }

    bool isHighScoreSelected(Vector2f mousePos) {
        return highScoreButton->contains(mousePos);
    }

    bool isExitSelected(Vector2f mousePos) {
        return exitButton->contains(mousePos);
    }

    bool isReturnToMenuSelected(Vector2f mousePos) {
        return returnToMenuButton->contains(mousePos);
    }

    bool isMusicOffSelected(Vector2f mousePos) {
        return musicOffButton->contains(mousePos);
    }

    void openInstructions() {
        Texture instructionsTexture;
        Sprite instructionsSprite;
        if (instructionsTexture.loadFromFile("Images/Instructions.jpg")) {
            instructionsSprite.setTexture(instructionsTexture);
            float scaleX = static_cast<float>(window.getSize().x) / instructionsTexture.getSize().x;
            float scaleY = static_cast<float>(window.getSize().y) / instructionsTexture.getSize().y;
            instructionsSprite.setScale(scaleX, scaleY);

            window.clear();
            window.draw(instructionsSprite);
            window.display();
            showInstructions = true;
            while (window.isOpen() && showInstructions) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed) {
                        window.close();
                        break;
                    }
                    else if (e.type == Event::MouseButtonPressed) {
                        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                        if (mousePos.y > 600 && mousePos.x > 500 && mousePos.x < 700 ) {
                            showInstructions = false;
                            break;
                        }
                    }
                }
            }
        }
        else {
            cout << "Failed to load instructions image!" << endl;
        }
    }

    void openendGame()
    {
        Texture endGameTexture;
        Sprite endGameSprite;
        if (endGameTexture.loadFromFile("Images/button_menus/gameover.jpeg")) {
            endGameSprite.setTexture(endGameTexture);
            float scaleX = static_cast<float>(window.getSize().x) / endGameTexture.getSize().x;
            float scaleY = static_cast<float>(window.getSize().y) / endGameTexture.getSize().y;
            endGameSprite.setScale(scaleX, scaleY);

            window.clear();
            window.draw(endGameSprite);
            window.display();

            while (window.isOpen()) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed) {
                        window.close();
                        break;
                    }
                }
            }
        }
        else {
            cout << "Failed to load end game image!" << endl;
        }
    }

    void openLevelWon()
    {
        Texture levelWonTexture;
        Sprite levelWonSprite;
        if (levelWonTexture.loadFromFile("Images/button_menus/levelcleared.png")) {
            levelWonSprite.setTexture(levelWonTexture);
            float scaleX = static_cast<float>(window.getSize().x) / levelWonTexture.getSize().x;
            float scaleY = static_cast<float>(window.getSize().y) / levelWonTexture.getSize().y;
            levelWonSprite.setScale(scaleX, scaleY);

            window.clear();
            window.draw(levelWonSprite);
            window.display();

            while (window.isOpen()) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed) {
                        window.close();
                        break;
                    }
                }
            }
        }
        else {
            cout << "Failed to load level won image!" << endl;
        }
    }
};