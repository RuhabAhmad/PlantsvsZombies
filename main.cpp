#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "linker.h"
using namespace sf;
using namespace std;

int main() {
    RenderWindow window(VideoMode(1200, 700), "Menu");
    MainMenu menu(window);

    // Audio
    Music menuMusic;
    menuMusic.openFromFile("Audios/menu.mp3");
    menuMusic.play();
    Game game;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                if (menu.isPlay1Selected(mousePos)) {
                    window.clear();
                    menuMusic.stop();
                    window.display();
                    game.run1();
                    menu.openendGame();
                }
                else if (menu.isPlay2Selected(mousePos)) {
                    window.clear();
                    menuMusic.stop();
                    window.display();
                    game.run2();
                    menu.openendGame();
                }
                else if (menu.isPlay3Selected(mousePos)) {
                    window.clear();
                    menuMusic.stop();
                    window.display();
                    game.run3();
                    menu.openendGame();
                    
                }
                else if (menu.isHighScoreSelected(mousePos)) {
                    // Handle high score
                }
                else if (menu.isInstructionsSelected(mousePos)) {
                    menu.openInstructions();
                }
                else if (menu.isExitSelected(mousePos)) {
                    window.close();
                }
                else if (menu.isMusicOffSelected(mousePos)) {
                    menuMusic.stop();
                }
            }
        }

        window.clear();
        menu.draw();
        window.display();
    }
    return 0;
}