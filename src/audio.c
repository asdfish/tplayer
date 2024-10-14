#include <audio.h>

#include <soloud_c.h>

#include <stddef.h>

Soloud* soloud = NULL;
WavStream* wav_stream = NULL;

int audio_init(void) {
  soloud = Soloud_create();
  if(soloud == NULL)
    return -1;

  if(Soloud_init(soloud) != 0) {
    Soloud_destroy(soloud);
    return -1;
  }

  wav_stream = WavStream_create();
  if(wav_stream == NULL) {
    Soloud_deinit(soloud);
    Soloud_destroy(soloud);
    return -1;
  }

  return 0;
}

bool audio_is_playing(void) {
  return soloud != NULL && Soloud_getVoiceCount(soloud) > 0 ? true : false;
}

int audio_play(const char* path) {
  if(soloud == NULL || wav_stream == NULL || path == NULL)
    return -1;

  Soloud_stopAll(soloud);
  WavStream_stop(wav_stream);

  if(WavStream_load(wav_stream, path) != 0)
    return -1;
  Soloud_play(soloud, wav_stream);

  return 0;
}

void audio_uninit(void) {
  if(wav_stream != NULL)
    WavStream_destroy(wav_stream);

  if(soloud != NULL) {
    Soloud_deinit(soloud);
    Soloud_destroy(soloud);
  }
}
