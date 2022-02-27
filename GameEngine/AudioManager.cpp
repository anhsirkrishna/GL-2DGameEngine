/******************************************************************************/
/*!
/*File: AudioManager.cpp
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   2/14/2022
/*  Implementation of the Audio Manager Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "AudioManager.h"
#include <assert.h>

// default constructor
AudioManager::AudioManager() {
	FMOD_RESULT result;

	result = FMOD::System_Create(&system);

#if DEBUG
	if (result != FMOD_OK)
		printf("FMOD error! (%d) \n", result);
#endif

	result = system->init(512, FMOD_INIT_NORMAL, 0);

#if DEBUG
	if (result != FMOD_OK)
		printf("FMOD error! (%d) \n", result);
#endif

	result = system->getMasterChannelGroup(&master_channel);

#if DEBUG
	if (result != FMOD_OK)
		printf("FMOD error! (%d) \n", result);
#endif

	SetMasterVolume(0.1f);

	CreateSound("bass.wav");
}

// creates a sounds from a .wav file
void AudioManager::CreateSound(std::string filename) {

	std::string file_path = "..\\Resources\\";
	file_path = file_path + filename;

	FMOD::Sound* sample;
	FMOD_RESULT result;

	result = system->createSound(file_path.c_str(), FMOD_DEFAULT, 0, &sample);

#if DEBUG

	if (result != FMOD_OK)
		printf("FMOD error! (%d) \n", result);

#endif

	// puts the sound pointer in the map, with the filename as a key
	sound_map[filename] = sample;

}

// plays a sound, if it's already loaded in the sound map
void AudioManager::Play(std::string sound_name) const {

	FMOD_RESULT result;

	if (sound_map.at(sound_name))
	{
		result = system->playSound(sound_map.at(sound_name), 0, false, 0);
		assert(result != FMOD_OK);
	}
	else
	{
#if DEBUG
		printf("%s not in the sound map!", sound_name);
#endif
	}
}

// sets master volume in [0,1] range
void AudioManager::SetMasterVolume(float volume) {

	master_channel->setVolume(volume);
}

// updates FMOD::System
void AudioManager::Update() {
	system->update();
}

// deletes all sound pointers
void AudioManager::Cleanup() {
	/*
	* This code is supposed to delete the sound created by FMOD
	* But we haven't called new to create it
	* So I don't think we can call delete on it like this
	*
	for (auto& sound_entry : sound_map)
		delete sound_entry.second;
	*/

}

// default destructor
AudioManager::~AudioManager() {
	Cleanup();
}