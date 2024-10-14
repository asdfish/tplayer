CC ?= cc
C_STANDARD := -std=gnu99

CXX ?= c++
CXX_STANDARD := -std=c++98

INCLUDE_FLAGS := -Iinclude -Ideps/soloud/include -Ideps/termbox2
LINK_FLAGS := -Lbuild -lsoloud -lopenmpt

# "$(find deps/soloud/src -type f | grep '\.c' | grep -v 'backend' | grep -v 'tools') deps/soloud/src/backend/miniaudio/soloud_miniaudio.cpp"
SOLOUD_SOURCE_FILES := deps/soloud/src/audiosource/ay/chipplayer.cpp $\
											 deps/soloud/src/audiosource/ay/sndbuffer.cpp $\
											 deps/soloud/src/audiosource/ay/sndchip.cpp $\
											 deps/soloud/src/audiosource/ay/sndrender.cpp $\
											 deps/soloud/src/audiosource/ay/soloud_ay.cpp $\
											 deps/soloud/src/audiosource/monotone/soloud_monotone.cpp $\
											 deps/soloud/src/audiosource/noise/soloud_noise.cpp $\
											 deps/soloud/src/audiosource/openmpt/soloud_openmpt.cpp $\
											 deps/soloud/src/audiosource/openmpt/soloud_openmpt_dll.c $\
											 deps/soloud/src/audiosource/sfxr/soloud_sfxr.cpp $\
											 deps/soloud/src/audiosource/speech/darray.cpp $\
											 deps/soloud/src/audiosource/speech/klatt.cpp $\
											 deps/soloud/src/audiosource/speech/resonator.cpp $\
											 deps/soloud/src/audiosource/speech/soloud_speech.cpp $\
											 deps/soloud/src/audiosource/speech/tts.cpp $\
											 deps/soloud/src/audiosource/tedsid/sid.cpp $\
											 deps/soloud/src/audiosource/tedsid/soloud_tedsid.cpp $\
											 deps/soloud/src/audiosource/tedsid/ted.cpp $\
											 deps/soloud/src/audiosource/vic/soloud_vic.cpp $\
											 deps/soloud/src/audiosource/vizsn/soloud_vizsn.cpp $\
											 deps/soloud/src/audiosource/wav/dr_impl.cpp $\
											 deps/soloud/src/audiosource/wav/soloud_wav.cpp $\
											 deps/soloud/src/audiosource/wav/soloud_wavstream.cpp $\
											 deps/soloud/src/audiosource/wav/stb_vorbis.c $\
											 deps/soloud/src/backend/miniaudio/soloud_miniaudio.cpp $\
											 deps/soloud/src/c_api/soloud_c.cpp $\
											 deps/soloud/src/core/soloud.cpp $\
											 deps/soloud/src/core/soloud_audiosource.cpp $\
											 deps/soloud/src/core/soloud_bus.cpp $\
											 deps/soloud/src/core/soloud_core_3d.cpp $\
											 deps/soloud/src/core/soloud_core_basicops.cpp $\
											 deps/soloud/src/core/soloud_core_faderops.cpp $\
											 deps/soloud/src/core/soloud_core_filterops.cpp $\
											 deps/soloud/src/core/soloud_core_getters.cpp $\
											 deps/soloud/src/core/soloud_core_setters.cpp $\
											 deps/soloud/src/core/soloud_core_voicegroup.cpp $\
											 deps/soloud/src/core/soloud_core_voiceops.cpp $\
											 deps/soloud/src/core/soloud_fader.cpp $\
											 deps/soloud/src/core/soloud_fft.cpp $\
											 deps/soloud/src/core/soloud_fft_lut.cpp $\
											 deps/soloud/src/core/soloud_file.cpp $\
											 deps/soloud/src/core/soloud_filter.cpp $\
											 deps/soloud/src/core/soloud_misc.cpp $\
											 deps/soloud/src/core/soloud_queue.cpp $\
											 deps/soloud/src/core/soloud_thread.cpp $\
											 deps/soloud/src/filter/soloud_bassboostfilter.cpp $\
											 deps/soloud/src/filter/soloud_biquadresonantfilter.cpp $\ $\
											 deps/soloud/src/filter/soloud_dcremovalfilter.cpp $\
											 deps/soloud/src/filter/soloud_duckfilter.cpp $\
											 deps/soloud/src/filter/soloud_echofilter.cpp $\
											 deps/soloud/src/filter/soloud_eqfilter.cpp $\
											 deps/soloud/src/filter/soloud_fftfilter.cpp $\
											 deps/soloud/src/filter/soloud_flangerfilter.cpp $\
											 deps/soloud/src/filter/soloud_freeverbfilter.cpp $\
											 deps/soloud/src/filter/soloud_lofifilter.cpp $\
											 deps/soloud/src/filter/soloud_robotizefilter.cpp $\
											 deps/soloud/src/filter/soloud_waveshaperfilter.cpp

TERMBOX2_FLAGS := -DTB_IMPL -w

PPLAYER_OBJECT_FILES := build/termbox2.h.o $\
												build/audio.c.o build/directory.c.o build/global.c.o build/main.c.o build/menu.c.o

DEBUG_FLAGS := -Wall -Wextra -Wpedantic
OPTIMIZATION_FLAGS :=

TARGET := release
# TARGET := debug

define C_COMPILE
	${CC} -xc -c $(1) ${C_STANDARD} ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o $(2)

endef

define SOLOUD_COMPILE
	${CXX} -xc++ -c $(1) ${CXX_STANDARD} -iquote deps/soloud/include -w ${OPTIMIZATION_FLAGS} -o build/soloud/$(notdir $(1)).o -DWITH_MINIAUDIO

endef

# setup
all: build deps optimize tplayer

optimize:
ifeq (${TARGET}, release)
	$(eval OPTIMIZATION_FLAGS:= -Os -march=native -flto=auto)
	$(eval LINK_FLAGS += -flto=auto)
else
	$(eval OPTIMIZATION_FLAGS:= -Og -g)
endif

build:
	mkdir build

build/soloud:
	mkdir build/soloud

build/libsoloud.a: build/soloud deps/soloud
	$(foreach SOLOUD_SOURCE_FILE, $\
		${SOLOUD_SOURCE_FILES}, $\
		$(call SOLOUD_COMPILE, ${SOLOUD_SOURCE_FILE}) $\
	)
	ar rcs build/libsoloud.a build/soloud/*

deps:
	mkdir deps

deps/soloud:
	git -C deps clone https://github.com/jarikomppa/soloud --depth=1

deps/termbox2:
	git -C deps clone https://github.com/termbox/termbox2 --depth=1

build/termbox2.h.o: deps/termbox2 deps/termbox2/termbox2.h deps/termbox2/LICENSE
	$(call C_COMPILE,deps/termbox2/termbox2.h,${TERMBOX2_FLAGS})

build/audio.c.o: include/audio.h src/audio.c
	$(call C_COMPILE,src/audio.c)

build/directory.c.o: include/directory.h src/directory.c
	$(call C_COMPILE,src/directory.c)

build/global.c.o: include/global.h include/global.h src/global.c
	$(call C_COMPILE,src/global.c)

build/main.c.o: include/global.h src/main.c
	$(call C_COMPILE,src/main.c)

build/menu.c.o: include/global.h include/menu.h src/menu.c
	$(call C_COMPILE,src/menu.c)

tplayer: build build/libsoloud.a ${PPLAYER_OBJECT_FILES}
	${CXX} ${PPLAYER_OBJECT_FILES} ${LINK_FLAGS} -o tplayer
ifeq (${TARGET}, release)
	strip tplayer
endif

# utils
clean:
ifneq (, $(wildcard build))
	rm -rf build
endif
ifneq (, $(wildcard deps))
	rm -rf deps
endif
ifneq (, $(wildcard tplayer))
	rm -f tplayer
endif

run: all
	./tplayer

test: all
	valgrind --gen-suppressions=all --log-file=valgrind_log.txt ./tplayer
