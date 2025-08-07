#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"

const std::string RESOURCES_PATH = "Resources/";

int main()
{
    sf::RenderWindow window(sf::VideoMode(880, 600), "Snake Game"); // Обновили размеры окна

    SnakeGame::Game game;

    sf::Clock clock;
    float timer = 0.0f;

    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                game.handleInput(event.key.code);
        }

        if (timer >= SnakeGame::UPDATE_INTERVAL)
        {
            game.update();
            timer = 0;
        }

        game.draw(window);
    }

    return 0;
}
