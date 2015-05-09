#include "sound.h"


std::map<ALuint, SndInfo> Buffers;
ALCdevice *pDevice;
ALCcontext *pContext;
pthread_attr_t attr;

bool InitializeOpenAL()
{
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

    pDevice = alcOpenDevice(NULL);

    if (!pDevice)
    {
        perror("Default sound device not present");
        return false;
    }

    pContext = alcCreateContext(pDevice, NULL);
    if (!CheckALCError()) return false;

    alcMakeContextCurrent(pContext);

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);

    int s = pthread_attr_init(&attr);
	assert(s == 0);

    return true;
}

void DestroyOpenAL()
{
  for (std::map<ALuint, SndInfo>::iterator i = Buffers.begin(); i != Buffers.end(); i++)
    alDeleteBuffers(1, &i->second.ID);

  alcMakeContextCurrent(NULL);
  alcDestroyContext(pContext);
  alcCloseDevice(pDevice);

  pthread_attr_destroy(&attr);
}

ALboolean CheckALCError()
{
  ALenum ErrCode;
  std::string Err = "ALC error: ";
  if ((ErrCode = alcGetError(pDevice)) != ALC_NO_ERROR)
  {
    Err += (char *)alcGetString(pDevice, ErrCode);
    perror(Err.data());
    return AL_FALSE;
  }
  return AL_TRUE;
}

ALboolean CheckALError()
{
  ALenum ErrCode;
  std::string Err = "OpenAL error: ";
  if ((ErrCode = alGetError()) != AL_NO_ERROR)
  {
    Err += (char *)alGetString(ErrCode);
    perror(Err.data());
    return AL_FALSE;
  }
  return AL_TRUE;
}

static void *play_thread(void *media_id) //void * == longint
{
	unsigned mSourceID = *((unsigned *)media_id);
    free(media_id);

	alSourcePlay(mSourceID);
}

unsigned remSnd::Open(const std::string &Filename, bool Looped, bool Streamed)
{
  std::ifstream a(Filename.c_str());
  if (!a.is_open()) return false;
  a.close();

  mLooped    = Looped;

  alGenSources(1, &mSourceID);
  if (!CheckALError()) return false;

  alSourcef (mSourceID, AL_PITCH,    1.0f);
  alSourcef (mSourceID, AL_GAIN,    1.0f);
  alSourcefv(mSourceID, AL_POSITION,  mPos);
  alSourcefv(mSourceID, AL_VELOCITY,  mVel);
  alSourcei (mSourceID, AL_LOOPING,  mLooped);

  if (LoadWavFile(Filename)) return mSourceID; else return 0;
}

void remSnd::Play()
{
    unsigned *info = (unsigned *)calloc(1, sizeof(unsigned));
    *info = mSourceID;

	pthread_t thread_id;
	pthread_create(&thread_id, &attr, &play_thread, info);
}

void remSnd::Play(unsigned sound_id)
{
    unsigned *info = (unsigned *)calloc(1, sizeof(unsigned));
    *info = sound_id;

	pthread_t thread_id;
	pthread_create(&thread_id, &attr, &play_thread, (void *)info);
}

void remSnd::Close()
{
  alSourceStop(mSourceID);
  if (alIsSource(mSourceID)) alDeleteSources(1, &mSourceID);
}

void remSnd::Stop()
{
  alSourceStop(mSourceID);
}

void remSnd::Move(float X, float Y, float Z)
{
  ALfloat Pos[3] = { X, Y, Z };
  alSourcefv(mSourceID, AL_POSITION, Pos);
}

bool remSnd::LoadWavFile(const std::string &Filename)
{
  SndInfo    buffer;
  ALenum    format;
  ALvoid    *data;
  ALsizei    size;
  ALsizei    freq;
  ALboolean  loop;
  ALuint    BufID = 0;

  buffer.Filename = Filename;
  for (std::map<ALuint, SndInfo>::iterator i = Buffers.begin(); i != Buffers.end(); i++)
  {
    if (i->second.Filename == Filename) BufID = i->first;
  }

  if (!BufID)
  {
    alGenBuffers(1, &buffer.ID);
    if (!CheckALError()) return false;
    alutLoadWAVFile((ALbyte *)Filename.data(), &format, &data,
                     &size, &freq, &loop);
    if (!CheckALError()) return false;

    buffer.Format      = format;
    buffer.Rate      = freq;
    alBufferData(buffer.ID, format, data, size, freq);
    if (!CheckALError()) return false;
    alutUnloadWAV(format, data, size, freq);
    if (!CheckALError()) return false;

    Buffers[buffer.ID] = buffer;
  }
  else
    buffer = Buffers[BufID];

  alSourcei (mSourceID, AL_BUFFER, buffer.ID);

  return true;
}


