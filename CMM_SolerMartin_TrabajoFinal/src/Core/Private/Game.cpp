//C++ includes
#include <iostream>

//SFML Include
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
//Custom includes
#include "Game.h"
#include "MenuScene.h"
#include <AudioManager.h>
Game::Game()
    : window(sf::VideoMode({ 1280, 720 }), "Quack Attack: Reloaded", sf::Style::None)
{
    window.setFramerateLimit(60);
    sceneManager.PushScene(std::make_unique<MenuScene>(window, resources, sceneManager));

    AudioManager::Get().Init(resources);

    AudioManager::Get().PlayMusic("MenuMusic", true);
}

void Game::RunGame()
{
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        if (!sceneManager.hasScene()) break;

        Scene* current = sceneManager.GetCurrentScene();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                continue;
            }
            current->HandleEvent(*event);
        }

        current->Update(deltaTime);
        if (!current->GetRequestedScene().empty())
        {
            std::string next = current->GetRequestedScene();
            current->ClearRequestedScene();

            if (next == "MainMenu")
            {
                sceneManager.SwitchScene(std::make_unique<MenuScene>(window, resources, sceneManager));
                continue;
            }
            else if (next == "ResumeGameplay")
            {
                sceneManager.PopScene();
                continue;
            }
        }
        current->Render();
    }
}
