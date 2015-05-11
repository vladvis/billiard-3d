#ifndef SOUND
#define SOUND

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

typedef struct
{
  unsigned int  ID;
  std::string    Filename;
  unsigned int  Rate;
  unsigned int  Format;
} SndInfo;

class remSnd
{
	public:
	  ALfloat mVel[3];
	  ALfloat mPos[3];
	  bool  mLooped;
	  ALuint    mSourceID = 0;

	  unsigned Open(const std::string &Filename, bool Looped, bool Streamed);
	  bool IsStreamed();
	  void Play();
	  void Play(float sound_volume);
	  void Play(unsigned sound_id);
	  void Play(unsigned sound_id, float sound_volume);
	  void Close();
	  void Update();
	  void Move(float X, float Y, float Z);
	  void Stop();

	private:

		bool      mStreamed;

		bool LoadWavFile (const std::string &Filename);
};

ALboolean CheckALCError();
ALboolean CheckALError();
bool InitializeOpenAL();
void DestroyOpenAL();

#endif //SOUND

