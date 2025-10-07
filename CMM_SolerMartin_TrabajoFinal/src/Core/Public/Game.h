#pragma once

//C++
#include <memory>
//SFML
#include <SFML/Graphics.hpp>
//Custom
#include "SceneManager.h"
#include "ResourcesManager.h"



class Game
{
public:

    Game();
    void RunGame();

private:
    sf::RenderWindow window;
    SceneManager sceneManager;
    ResourcesManager resources;

};

