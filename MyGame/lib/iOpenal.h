#pragma once

//#include <AL/al.h>
//#include <AL/alc.h>

#include <al.h>
#include <alc.h>
#pragma comment(lib, "OpenAL32")

class iOpenal
{
public:
	iOpenal(int audioNum);
	virtual ~iOpenal();

	void initBuffer(int idx, const char* szFormat, ...);
	void initSource(int idx, bool repeat);

	void play(int idx, bool repeat);
	void pause(int idx);
	void stop(int idx);
	void volume(int idx, float vol);
	void pitch(int idx, float pitch);

public:
	ALuint*	gBuffer;
	ALuint*	gSource;
	int bufCount;
};

typedef struct _AudioInfo
{
	char* fileName;
	bool bRepeat;
	float volume;
	float pitch;
}AudioInfo;

void loadAudio(AudioInfo* ai, int aiNum);
void freeAudio();
void pauseAudio();
void resumeAudio();
void audioPlay(int sndIdx);
void audioStop();
void audioVolume(float bgm, float eff, int effNum);
void audioPitch(int sndIdx, float pitch);

