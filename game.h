#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "linker.h"
using namespace sf;
using namespace std;

class Game {
private:
    RenderWindow window;
    bool levelFailed;

public:
    Game() : window(), levelFailed(false) {}

    void run1()
    {
        Level1 L1(window);
        L1.run();

        if (L1.isEnd())
        {
            cout << "lost1" << endl;
            levelFailed = true;
        }

        if (L1.isWin())
        {
            cout << "Won" << endl;
            Level2 L2(window);
            L2.run();
        }
    }

    void run2()
    {
        Level2 L2(window);
        L2.run();

        if (L2.isEnd())
        {
            cout << "lost2" << endl;
        }

        if (L2.isWin())
        {
            cout << "Won" << endl;
            Level3 L3(window);
            L3.run();
        }
    }

    void run3()
    {
        Level3 L3(window);
        L3.run();

        if (L3.isEnd())
        {
            cout << "lost3" << endl;
        }

        if (L3.isWin())
        {
            cout << "Won" << endl;
        }
    }   
};
        
