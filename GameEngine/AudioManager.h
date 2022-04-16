/******************************************************************************/
/*!
/*File: AudioManager.h
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   2/14/2022
/*  Definition of the Audio Manager Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once
#include <fmod.hpp>
#include <map>
#include <string>

// Custom FMOD Wrapper Class
class AudioManager
{
	FMOD::System* system;
	FMOD::ChannelGroup* master_channel;
	std::map<std::string, FMOD::Sound*> sound_map;
	float master_volume;
public:

	AudioManager();
	~AudioManager();
	void CreateSound(std::string filename);
	void CreateLoopingSound(std::string filename);
	void Play(std::string sound_name) const;
	void SetMasterVolume(float volume);
	void StopAllSounds();
	void ToggleMute();
	void Update();
	void Cleanup();
};

//Declared as an extern variable so it can be accessed throughout the project
extern AudioManager* p_audio_manager;

