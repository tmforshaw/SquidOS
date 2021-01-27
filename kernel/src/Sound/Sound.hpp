#pragma once
#include "../Types/IntDef.hpp"

class SoundManager
{
private:
public:
	void PlayFrequency( uint32_t frequency );
	void StopSound();
	void Beep();
};

extern SoundManager* GlobalSound;