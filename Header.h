#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
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

class MainMenu {
public:
    MainMenu(RenderWindow& window) : window(window), showInstructions(false) {
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

        playButton = new PlayButton(buttonTexture, font, "PLAY", Vector2f(50, 550));
        instructionsButton = new InstructionsButton(buttonTexture, font, "INSTRUCTIONS", Vector2f(50, 600));
        exitButton = new ExitButton(buttonTexture, font, "EXIT", Vector2f(50, 650));
        returnToMenuButton = new Button(buttonTexture, font, "RETURN TO MAIN MENU", Vector2f(50, 600));
    }

    ~MainMenu() {
        delete playButton;
        delete instructionsButton;
        delete exitButton;
        delete returnToMenuButton;
    }

    void draw() {
        window.draw(backgroundSprite);
        playButton->draw(window);
        instructionsButton->draw(window);
        exitButton->draw(window);
        if (showInstructions) {
            returnToMenuButton->draw(window);
        }
    }

    bool isPlaySelected(Vector2f mousePos) {
        return playButton->contains(mousePos);
    }

    bool isInstructionsSelected(Vector2f mousePos) {
        return instructionsButton->contains(mousePos);
    }

    bool isExitSelected(Vector2f mousePos) {
        return exitButton->contains(mousePos);
    }

    bool isReturnToMenuSelected(Vector2f mousePos) {
        return returnToMenuButton->contains(mousePos);
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
                        if (isReturnToMenuSelected(mousePos)) {
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

private:
    RenderWindow& window;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture buttonTexture;
    Font font;
    Button* playButton;
    Button* instructionsButton;
    Button* exitButton;
    Button* returnToMenuButton;
    bool showInstructions;
};


#define MAX_SUNLIGHTS 5 // Maximum number of sunlights
#define MAX_PLANTS 20 // Maximum number of plants
#define MAX_PEA 100 // Maximum number of peas
#define MAX_SUNLIGHT 100 // Maximum number of sunlight instances

struct Sunlight {
    bool isActive = false; // Indicates if sunlight is active
    float position[2] = { 0, 0 }; // Array to store positions of sunlights
    Clock timer; // Timer for sunlight interval
};

class Score {
private:
    Texture sunTexture;
    Sprite sunSprite;
    Font font;
    Text scoreText;

public:
    int scoreValue;
    Score() : scoreValue(0) {
        // Load sun texture
        if (!sunTexture.loadFromFile("sun.png")) {
            cout << "Failed to load sun image!" << endl;
        }
        sunSprite.setTexture(sunTexture);
        sunSprite.setScale(0.1f, 0.1f); // Scale down the sun image if needed
        sunSprite.setPosition(50, 50); // Adjust position as needed

        // Load font for score text
        if (!font.loadFromFile("futura_bold.ttf")) {
            cout << "Failed to load font!" << endl;
        }
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color::Green);
        scoreText.setPosition(120, 10); // Adjust position as needed
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
        // Draw sun image and score text
        window.draw(sunSprite);
        window.draw(scoreText);
    }
};

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

    virtual void dummy() {}

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        planted = true;
    }

    void draw(RenderWindow& window) {
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
};

struct InventoryItem {
    bool isActive = false;
    float position[2] = { -100, -100 };
    Texture itemTexture;
    Sprite itemSprite;
    Plant* P;
    //Sprite pea[MAX_PEA];
    Clock timer;
};

class Sunflower : public Plant {
private:
    Sprite sunlight[MAX_SUNLIGHT];
    Clock sunlightTimer;

public:
    Sunflower(float scaleX = 0.5f, float scaleY = 0.5f) : Plant("Images/img2.png", "sunflower", 200, 0, 100) {
        sprite.setScale(scaleX, scaleY);
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
        if (peaTimer.getElapsedTime().asSeconds() > 3) {
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
};


class Zombie {
public:
    int health;
    int attack;
    Texture texture;
    Sprite sprite;
    bool spawned;

    Zombie(const string& file, int health, int attack) : health(health), attack(attack) {
        if (!texture.loadFromFile(file)) {
            cout << "Could not load zombie texture." << endl;
            return;
        };
        sprite.setTexture(texture);
        sprite.setScale(1.4f, 1.4f); // Adjust scale as needed
        spawned = false;
    }

    void setTexture(const string& file) {
        texture.loadFromFile(file);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }

    void draw(RenderWindow& window)
    {
        if (health != 0)
            window.draw(sprite);
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
};

class SimpleZombie : public Zombie {
public:
    SimpleZombie(float scaleX = 1.0f, float scaleY = 1.0f) : Zombie("Images/simple.png", 100, 100) {
        sprite.setScale(scaleX, scaleY);
    }

    void setPosition(int x, int y) {
        sprite.setPosition(x, y);
        spawned = true;
    }
};

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
    Music peaCollision;

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

        // Load music
        peaCollision.openFromFile("Audios/zombiehit.mp3");

        numZombies = 0;
        lastZombiePositionY = 0;
    };

    void spawnZombie(int x, int y) {
        y = 2 * 110 + 85;
        if (zombieTimer.getElapsedTime().asSeconds() > 18) {
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

    // Collision between zombie and pea
    void handleCollisions(InventoryItem items[MAX_PLANTS], SimpleZombie zombies[], const int maxZombies) {
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
                                    peaCollision.play();
                                    peas[j].setPosition(0, 0); // Remove the pea
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void handleZombieDeaths(SimpleZombie zombies[], const int maxZombies) {
        for (int i = 0; i < maxZombies; i++) {
            if (zombies[i].spawned) {
                if (zombies[i].health <= 0) {
                    zombies[i].spawned = false; // Zombie died, so set spawned to false
                }
            }
        }
    }

    void handleGameLogic(InventoryItem items[MAX_PLANTS], SimpleZombie zombies[], const int maxZombies) {
        handleCollisions(items, zombies, maxZombies);
        handleZombieDeaths(zombies, maxZombies);
    }
};

class Game {
private:
    RenderWindow window;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Level1 L1;
    Texture selectedItemTexture;
    int selectedItemIndex = -1;
    Music backgroundMusic;

    enum GameState {
        Idle,
        PlacingItem
    };

    GameState gameState = Idle;

public:

    Game() : window(VideoMode(1200, 700), "GTA VI"), L1(window) {
        backgroundMusic.openFromFile("Audios/background.mp3");
        backgroundMusic.play();
    };

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

    bool handleSunlightClicks(Sunlight sunlights[MAX_SUNLIGHTS], const int mouseX, const int mouseY, int& score, bool x, Level1& level) {
        bool sunlightClicked = false;

        // Check if any sunlight was clicked
        for (int i = 0; i < MAX_SUNLIGHTS; i++) {
            if (sunlights[i].isActive) {
                // Check if the mouse click intersects with the sunlight
                if (mouseX >= sunlights[i].position[0] && mouseX <= sunlights[i].position[0] + 50 &&
                    mouseY >= sunlights[i].position[1] && mouseY <= sunlights[i].position[1] + 50) {
                    sunlights[i].isActive = false;
                    score += 25;
                    sunlightClicked = true;
                    break;
                }
            }
        }

        return sunlightClicked;
    }

    void run() {
        // Draw background.
        window.draw(backgroundSprite);

        srand(time(NULL));
        RenderWindow window(VideoMode(1200, 700), "Plants Vs Zombies");

        Texture sunlightTexture;
        if (!sunlightTexture.loadFromFile("sun.png")) {
            cout << "Failed to load sunlight image!" << endl;
            return;
        }
        Texture sunflowerTexture;
        if (!sunflowerTexture.loadFromFile("Images/img2.png")) {
            cout << "Failed to load sunflower image!" << endl;
            return;
        }
        Texture peashooterTexture;
        if (!peashooterTexture.loadFromFile("Images/peashooter.png")) {
            cout << "Failed to load peashooter image!" << endl;
            return;
        }
        Texture peaTexture;
        if (!peaTexture.loadFromFile("Images/pea.png")) {
            cout << "Failed to load pea image!" << endl;
            return;
        }

        // Create an array of zombies

        Level1 level1(window);
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
                else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;

                    if (gameState == PlacingItem) {
                        // If placing item, place the selected item at mouse coordinates
                        if (selectedItemIndex != -1) {
                            items[selectedItemIndex].position[0] = mouseX - 50;
                            items[selectedItemIndex].position[1] = mouseY - 50;

                            // Reset selected item index and game state
                            selectedItemIndex = -1;
                            gameState = Idle;
                        }
                    }
                    else {
                        int* currentScore = &score.scoreValue;
                        handleSunlightClicks(sunlights, mouseX, mouseY, *currentScore, false, level1);

                        for (int i = 0; i < MAX_PLANTS; i++) {
                            if (items[i].isActive && items[i].P->type == "sunflower") {
                                Sunflower* sunflower = dynamic_cast<Sunflower*>(items[i].P);
                                if (sunflower) {
                                    sunflower->checkSunlightClick(mouseX, mouseY, *currentScore);
                                }
                            }
                        }

                        selectedItemIndex = level1.handleInventoryItemClicks(window, items, mouseX, mouseY, *currentScore, false, level1, sunflowerTexture, peashooterTexture);

                        // If an inventory item is selected, change game state to PlacingItem
                        if (selectedItemIndex != -1) {
                            gameState = PlacingItem;
                        }
                    }
                }
            }

            // spawn handling.
            handleSunlight(sunlights, window.getSize().y);

            score.update();
            window.clear();

            // backgrounds.
            createBack(window);
            level1.createMap(window);

            drawSunlight(window, sunlights, sunlightTexture);
            level1.drawItem(window, items, selectedItemIndex);

            // zombies.
            level1.spawnZombie(1200, rand() % 550);
            level1.moveZombies();
            level1.drawZombies(window);

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

            level1.handleGameLogic(items, level1.zombies, Level1::MAX_ZOMBIES);

            // Handle collisions between peas and zombies
            level1.handleCollisions(items, level1.zombies, Level1::MAX_ZOMBIES);

            // Handle zombie deaths
            level1.handleZombieDeaths(level1.zombies, Level1::MAX_ZOMBIES);

            score.draw(window);
            level1.drawInventory(window);
            window.display();
        }
    }
};