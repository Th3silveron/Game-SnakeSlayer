#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Menu.h"

int main()
{
    using namespace SnakeGame;

    // Window setup with game grid dimensions plus HUD space
    int windowWidth = (GRID_WIDTH + HUD_WIDTH) * CELL_SIZE;
    int windowHeight = GRID_HEIGHT * CELL_SIZE;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SnakeSlayer Game");

    // Core systems initialization
    AudioManager audio;
    Menu menu(windowWidth, windowHeight, audio);
    Game* game = nullptr;  

    // State management variables
    bool inMenu = true;
    sf::Clock clock;
    float timeSinceLastUpdate = 0.f;

    audio.playMenuMusic();

    // Main game loop
    while (window.isOpen())
    {
        // Event handling for both menu and game states
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Menu navigation and selection logic
            if (inMenu)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (menu.handleInput(event.key.code))
                    {
                        int selected = menu.getSelectedItem();
                        if (menu.getState() == MenuState::Main)
                        {
                            if (selected == 0)  // Start game selected
                            {
                                inMenu = false;
                                // Clean up previous game instance if exists
                                if (game != nullptr)
                                {
                                    delete game;
                                    game = nullptr;
                                }

                                // Initialize new game with selected settings
                                game = new Game(audio, menu);
                                game->setDifficulty(menu.getDifficulty());
                                audio.stopAllMusic();
                                audio.playGameMusic();
                            }
                            else if (selected == 4)  // Exit game selected
                            {
                                window.close();
                            }
                        }
                    }
                }
            }
            // Game controls handling
            else if (game != nullptr)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    game->handleInput(event.key.code);
                }
            }
        }

        // Game state update and rendering
        if (!inMenu && game != nullptr)
        {
          
            timeSinceLastUpdate += clock.restart().asSeconds();
            float updateInterval = game->getUpdateInterval();

            if (timeSinceLastUpdate > updateInterval && !game->isGameOverMenuActive() && !game->isShowingLeaderboard())
            {
                game->update();
                timeSinceLastUpdate = 0.f;
            }

            game->draw(window);

            // Check if we should return to main menu
            if (game->shouldReturnToMenu())
            {
                inMenu = true;
                audio.stopAllMusic();
                audio.playMenuMusic();
                delete game;
                game = nullptr;
            }
        }
        // Menu rendering when not in game
        else
        {
            menu.update();
            menu.draw(window);
        }
    }

   
    if (game != nullptr)
        delete game;

    return 0;
}
















