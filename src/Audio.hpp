#include <stdexcept>
#include <cmath>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#define BACKGROUND "./audios/background.mp3"
#define INTRO "./audios/Intro.wav"
#define MOVE "./audios/Move.wav"

void audioCallback(void *userdata, Uint8 *stream, int len);

using namespace std;

//***************************************************************************************************************//
class AudioWAV
{
private:
	Uint8 *audio;
	Uint32 length;
	Uint32 remaining;
	int LoadWAV(string path, SDL_AudioSpec **wav_spec );
	int nextPart(int len);
	void copyMemo(Uint8 *stream, int len);
	void mix(Uint8 *stream, int len);

public:
	AudioWAV(string path, SDL_AudioSpec **wav_spec);
	friend class AudioManager;
};
AudioWAV::AudioWAV(string path, SDL_AudioSpec **wav_spec)
{
	if(LoadWAV(path, wav_spec ) == -1)
		throw invalid_argument("audio wave error!");
}
void AudioWAV::copyMemo(Uint8 *stream, int len)
{
	SDL_memcpy (stream, audio + (length - remaining), len);
}
void AudioWAV::mix(Uint8 *stream, int len)
{
	SDL_MixAudio(stream, audio + (length - remaining), len, SDL_MIX_MAXVOLUME);
	remaining -= len;
}

int AudioWAV::LoadWAV(string path, SDL_AudioSpec **wav_spec )
{
	if( SDL_LoadWAV(path.c_str(), *wav_spec, &audio, &length) == NULL )
		return -1;
	remaining = length;
	return 0;
}

int AudioWAV::nextPart(int len)
{
	int k = min((Uint32)len, remaining);
	if(remaining <= 0)
		remaining = length;
	return k;
}
//***************************************************************************************************************//
class AudioManager
{
private:
	Mix_Music *music;
	AudioWAV *intro, *move;
	bool introPlayed, movePlayed;
	int nextTracks(int len);
	void copyTracks(Uint8 *stream, int len);
	void mixTracks(Uint8 *stream, int len);
public:
	AudioManager();
	void playMusic();
	void pauseMusic();
	void playIntro();
	void playMove();
	friend void audioCallback(void *userdata, Uint8 *stream, int len);
};
AudioManager::AudioManager()
{
	introPlayed = false;
	movePlayed = false;

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return ;

	SDL_AudioSpec *wav_spec = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
	intro = new AudioWAV(INTRO, &wav_spec);
	move = new AudioWAV(MOVE, &wav_spec);
	wav_spec->callback = audioCallback;
	wav_spec->userdata = NULL;

	if ( SDL_OpenAudio(wav_spec, NULL) < 0 )
		exit(-1);

	SDL_PauseAudio(0);
	printf("\tPlaying Audios ...\n" );

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	int result = 0;
	int flags = MIX_INIT_MP3;

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("Failed to init SDL\n");
		exit(-1);
	}

	if (flags != (result = Mix_Init(flags)))
	{
		printf("Could not initialize mixer (result: %d).\n", result);
		printf("Mix_Init: %s\n", Mix_GetError());
		exit(1);
	}

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	music = Mix_LoadMUS(BACKGROUND);
	Mix_PlayMusic(music, -1);
	Mix_PauseMusic();
};

AudioManager audiomanager;

int AudioManager::nextTracks(int len)
{
	int tmp;
	if(movePlayed)
	{
		if((tmp = move->nextPart(len)) <= 0)
			movePlayed = false;
		else
			len = tmp;
	}
	if(introPlayed)
	{
		if((tmp = intro->nextPart(len)) <= 0)
			introPlayed = false;
		else
			len = tmp;
	}
	return len;
}

void AudioManager::copyTracks(Uint8 *stream, int len)
{
	if(movePlayed)
		move->copyMemo(stream, len);
	if(introPlayed)
		intro->copyMemo(stream, len);
}
void AudioManager::mixTracks(Uint8 *stream, int len)
{
	if(movePlayed)
		move->mix(stream, len);
	if(introPlayed)
		intro->mix(stream, len);
}

void AudioManager::playMusic()
{
	Mix_ResumeMusic();
}
void AudioManager::pauseMusic()
{
	Mix_PauseMusic();
}
void AudioManager::playIntro()
{
	introPlayed = true;
	SDL_PauseAudio(0);
}
void AudioManager::playMove()
{
	movePlayed = true;
	SDL_PauseAudio(0);
}

void audioCallback(void *userdata, Uint8 *stream, int len)
{
	len = audiomanager.nextTracks(len);
	audiomanager.copyTracks(stream, len);
	audiomanager.mixTracks(stream, len);
	if( !audiomanager.introPlayed && !audiomanager.movePlayed  )
		SDL_PauseAudio(1);
}
