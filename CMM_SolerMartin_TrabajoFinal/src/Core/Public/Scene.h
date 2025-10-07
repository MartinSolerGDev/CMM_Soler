#pragma once
//c++


//sfml
#include "SFML/Graphics.hpp"

class Scene
{
public:
	Scene(sf::RenderWindow& window);
	virtual ~Scene() = default;
	virtual void SetBackground(const sf::Texture& texture);

	virtual void HandleEvent(const sf::Event& event) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void RenderScene() = 0;
	virtual void Render();

	std::string GetRequestedScene() const;
	void ClearRequestedScene();

	virtual void OnEnterScene() = 0;
	virtual void OnExitScene() = 0;

protected:
	sf::RenderWindow& window;
	std::unique_ptr <sf::Sprite> background;

	void RequestSceneChange(const std::string& sceneName);

private:
	std::string requestedScene;
};