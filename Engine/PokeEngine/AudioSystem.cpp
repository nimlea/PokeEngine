#include "AudioSystem.h"
#include "..\..\Libs\OpenALFramework\Framework.h"
#include "LogSystem.h"

PE_BEGIN

AudioSystem::AudioSystem()
{}

AudioSystem::~AudioSystem()
{}

void AudioSystem::Init()
{
	ALFWInit();
	if (!ALFWInitOpenAL())
	{
		LogSystem::GetInstance().Log("Failed to initialize OpenAL");
		ALFWShutdown();
	}
}

void AudioSystem::Play(char *path)
{
	unsigned int sourceID;
	if (mAudioSource.find(path) == mAudioSource.end())
	{
		alGenSources(1, &sourceID);
		unsigned int bufferID;
		alGenBuffers(1, &bufferID);
		if (!ALFWLoadWaveToBuffer(path, bufferID))
		{
			LogSystem::GetInstance().Log("‘ÿ»Îaudioª∫¥Ê£∫%s  ß∞‹", path);
		}
		// attach
		alSourcei(sourceID, AL_BUFFER, bufferID);
		mAudioSource[path] = sourceID;
	}
	else
	{
		sourceID = mAudioSource[path];
	}
	alSourcePlay(sourceID);
}

PE_END