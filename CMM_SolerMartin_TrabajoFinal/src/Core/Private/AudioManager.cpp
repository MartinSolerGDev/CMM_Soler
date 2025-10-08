#include "AudioManager.h"
#include <iostream>

void AudioManager::Init(ResourcesManager& res)
{
	resources = &res;
	sf::Listener::setGlobalVolume(MasterVolume);

    sfxBuffers["DuckHit"] = &resources->GetSound("res/assets/Sounds/DuckHit.wav");
    sfxBuffers["GunShoot"] = &resources->GetSound("res/assets/Sounds/GunShoot.wav");
    sfxBuffers["NoAmmo"] = &resources->GetSound("res/assets/Sounds/NoAmmo.wav");
}

void AudioManager::SetVolume(VolumeType type, float val)
{
    val = std::clamp(val, 0.f, 100.f);
    switch (type)
    {
    case VolumeType::Master:
        MasterVolume = val;
        break;
    case VolumeType::Music:
        MusicVolume = val;
        break;
    case VolumeType::SFX:
        SFXVolume = val;
        break;
    }

    ApplyVolumes();

}

float AudioManager::GetVolume(VolumeType type) const
{
	switch (type)
	{
	case VolumeType::Master: return MasterVolume;
	case VolumeType::Music:  return MusicVolume;
	case VolumeType::SFX:    return SFXVolume;
	}
	return 100.f;

}

void AudioManager::PlayMusic(const std::string& id, bool loop)
{
    if (!resources) return;
    if (currentMusic && currentMusicId == id)
    {
        if (currentMusic->getStatus() != sf::Music::Status::Playing)
            currentMusic->play();

        return; 
    }

    auto newMusic = std::make_unique<sf::Music>();
    if (!newMusic->openFromFile("res/assets/Sounds/" + id + ".ogg"))
    {
        std::cerr << "Error cargando música: " << "res/assets/Sounds/" + id + ".ogg" << std::endl;
        return;
    }

    currentMusic = std::move(newMusic);
    currentMusicId = id;
    currentMusic->setLooping(loop);
    currentMusic->setVolume(MusicVolume * (MasterVolume / 100.f));
    currentMusic->play();
}

void AudioManager::StopMusic()
{
    if (currentMusic)
    {
        currentMusic->stop();
        currentMusic.reset();
        currentMusicId.clear();
    }
}

void AudioManager::PlaySFX(const std::string& id)
{
    if (!resources) return;

    auto it = sfxBuffers.find(id);
    if (it == sfxBuffers.end())
    {
        std::cerr << "SFX no encontrado: " << id << std::endl;
        return;
    }

    // Crear un sonido usando el buffer ya cargado en Init
    auto sound = std::make_unique<sf::Sound>(*it->second);
    sound->setVolume(SFXVolume * (MasterVolume / 100.f));
    sound->play();

    activeSounds.push_back(std::move(sound));

    // limpiar sonidos terminados
    activeSounds.erase(std::remove_if(activeSounds.begin(), activeSounds.end(), [](const std::unique_ptr<sf::Sound>& s) {return s->getStatus() == sf::Sound::Status::Stopped;}),activeSounds.end());
}

void AudioManager::ApplyVolumes()
{
    // Master
    sf::Listener::setGlobalVolume(MasterVolume);

    // Music
    if (currentMusic)
        currentMusic->setVolume(MusicVolume);

    // SFX
    for (auto& sound : activeSounds)
        if(sound)
            sound->setVolume(SFXVolume);

}
