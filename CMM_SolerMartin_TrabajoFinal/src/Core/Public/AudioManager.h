#pragma once
#include <SFML/Audio.hpp>
#include <ResourcesManager.h>

enum class VolumeType
{
	Master,
	Music,
	SFX
};

class AudioManager
{
public:
	static AudioManager& Get()
	{
		static AudioManager instance;
		return instance;
	}
	//Init resourcess
	void Init(ResourcesManager& res);

	//volume
	void SetVolume(VolumeType type, float val);
	float GetVolume(VolumeType type) const;
	// Music
	void PlayMusic(const std::string& id, bool loop = true);
	void StopMusic();

	// SFX
	void PlaySFX(const std::string& id);

private:
	AudioManager() = default;

	float MasterVolume = 100;
	float MusicVolume = 100;
	float SFXVolume = 100;

	ResourcesManager* resources = nullptr;

	//Music
	std::unique_ptr<sf::Music> currentMusic;
	std::string currentMusicId;

	// Buffers de SFX
	std::unordered_map<std::string, sf::SoundBuffer*> sfxBuffers;
	std::vector<std::unique_ptr<sf::Sound>> activeSounds;

	void ApplyVolumes();
};

