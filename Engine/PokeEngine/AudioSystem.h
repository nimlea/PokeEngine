#ifndef _AUDIOSYSTEM_H_
#define _AUDIOSYSTEM_H_

#include "..\Common\PEMacro.h"
#include <unordered_map>

PE_BEGIN

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();
	
	void Init();
	void Play(char *path);

private:
	std::unordered_map<char*, unsigned int> mAudioSource;
};

PE_END

#endif