#include "ResourcesManager.h"

ResourcesManager::ResourcesManager()
{

}

sf::Texture& ResourcesManager::GetTexture(const std::string path, const sf::IntRect rect)
{
	auto iterator = textures.find(path);

	if (iterator == textures.end())
	{
		sf::Texture* texture = new sf::Texture();

		if (!texture->loadFromFile(path, false, rect))
		{
			delete texture;
			throw std::runtime_error("No se puedo cargar textura " + path);
		}

		iterator = textures.emplace(path, texture).first;

	}
	return *iterator->second;
}

sf::SoundBuffer& ResourcesManager::GetSound(const std::string& path)
{
	auto iterator = sounds.find(path);

	if (iterator == sounds.end())
	{
		sf::SoundBuffer* sound = new sf::SoundBuffer();

		if (!sound->loadFromFile(path))
		{
			delete sound;
			throw std::runtime_error("No se puedo cargar sonido " + path);
		}

		iterator = sounds.emplace(path, sound).first;
	}
	return *iterator->second;
}

sf::Font& ResourcesManager::GetFont(const std::string& path)
{
	auto iterator = fonts.find(path);

	if (iterator == fonts.end())
	{
		sf::Font* font = new sf::Font();

		if (!font->openFromFile(path))
		{
			delete font;
			throw std::runtime_error("No se puedo cargar la fuente " + path);
		}

		iterator = fonts.emplace(path, font).first;
	}
	return *iterator->second;
}
