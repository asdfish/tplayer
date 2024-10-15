#include <audio.h>
#include <miniaudio.h>

ma_engine engine;
ma_sound sound;
bool started = false;

void sound_stop() {
  ma_sound_stop(&sound);
  ma_sound_uninit(&sound);
}

int audio_init() {
  if(ma_engine_init(NULL, &engine) != MA_SUCCESS)
    return -1;
  return 0;
}

int audio_play(const char* path) {
  if(started)
    sound_stop();

  if(ma_sound_init_from_file(&engine, path, MA_SOUND_FLAG_STREAM, NULL, NULL, &sound) != MA_SUCCESS)
    return -1;
  ma_sound_start(&sound);

  if(!started)
    started = true;
  return 0;
}

bool audio_is_playing() {
  if(!started)
    return false;

  return ma_sound_is_playing(&sound);
}

void audio_uninit() {
  if(started)
    sound_stop();

  ma_engine_stop(&engine);
  ma_engine_uninit(&engine);
}
