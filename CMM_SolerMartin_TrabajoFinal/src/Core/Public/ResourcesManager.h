#pragma once
//C++
#include <map>
#include <string>
//SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourcesManager
{
public:
	ResourcesManager();
	~ResourcesManager() = default;

	//Getters
	sf::Texture& GetTexture(const std::string path, const sf::IntRect rect);
	sf::SoundBuffer& GetSound(const std::string& path);
	sf::Font& GetFont(const std::string& path);

private:
	std::map<std::string, sf::Texture*> textures;
	std::map<std::string, sf::SoundBuffer*> sounds;
	std::map<std::string, sf::Font*> fonts;
};

