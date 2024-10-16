CC ?= cc
C_STANDARD := -std=gnu99

CXX ?= c++
CXX_STANDARD := -std=c++98

INCLUDE_FLAGS := -Iinclude -Ideps/miniaudio -Ideps/termbox2
LINK_FLAGS := -Lbuild -ldl -lpthread -lm

INSTALL_DIRECTORY := /usr/local/bin

MINIAUDIO_FLAGS := -DMA_IMPLEMENTATION $\
									 -DMA_NO_ENCODING $\
									 -DMA_NO_GENERATION -w
TERMBOX2_FLAGS := -DTB_IMPL -w

PPLAYER_OBJECT_FILES := build/miniaudio.h.o build/termbox2.h.o $\
												build/audio.c.o build/directory.c.o build/global.c.o build/main.c.o build/menu.c.o

DEBUG_FLAGS := -Wall -Wextra -Wpedantic
OPTIMIZATION_FLAGS :=

TARGET := release
# TARGET := debug

define C_COMPILE
	${CC} -xc -c $(1) ${C_STANDARD} ${INCLUDE_FLAGS} ${DEBUG_FLAGS} ${OPTIMIZATION_FLAGS} -o build/$(notdir $(1)).o $(2)

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

deps:
	mkdir deps

deps/miniaudio:
	git -C deps clone https://github.com/mackron/miniaudio --depth=1

deps/termbox2:
	git -C deps clone https://github.com/termbox/termbox2 --depth=1

build/miniaudio.h.o: deps/miniaudio
	$(call C_COMPILE,deps/miniaudio/miniaudio.h,${MINIAUDIO_FLAGS})

build/termbox2.h.o: deps/termbox2
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

tplayer: build ${PPLAYER_OBJECT_FILES}
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

install: all uninstall ${INSTALL_DIRECTORY}
	cp tplayer ${INSTALL_DIRECTORY}

uninstall:
ifneq (, $(wildcard ${INSTALL_DIRECTORY}/tplayer))
	rm ${INSTALL_DIRECTORY}/tplayer
endif
