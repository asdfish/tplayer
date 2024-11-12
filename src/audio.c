#include <audio.h>
#include <stdlib.h>
#include <miniaudio.h>

ma_engine engine;
ma_sound sound;
bool started = false;

inline void sound_stop(void) {
  ma_sound_stop(&sound);
  ma_sound_uninit(&sound);
}

int audio_init(void) {
  if(ma_engine_init(NULL, &engine) != MA_SUCCESS)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

bool audio_is_playing(void) {
  if(!started)
    return false;

  return ma_sound_is_playing(&sound);
}

int audio_play(const char* path) {
  if(started)
    sound_stop();

  if(ma_sound_init_from_file(&engine, path, MA_SOUND_FLAG_STREAM, NULL, NULL, &sound) != MA_SUCCESS)
    return EXIT_FAILURE;
  ma_sound_start(&sound);

  started = true;
  return EXIT_SUCCESS;
}

float audio_play_percentage(void) {
  if(!audio_is_playing())
    return 0.f;

  float length = 0.f;
  if(ma_sound_get_length_in_seconds(&sound, &length) != MA_SUCCESS)
    return 0.f;

  float cursor = 0.0f;
  if(ma_sound_get_cursor_in_seconds(&sound, &cursor) != MA_SUCCESS)
    return 0.f;

  if(length == 0.f || cursor == 0.f)
    return 0.f;

  return cursor / length;
}

void audio_uninit(void) {
  if(started)
    sound_stop();

  ma_engine_stop(&engine);
  ma_engine_uninit(&engine);
}
