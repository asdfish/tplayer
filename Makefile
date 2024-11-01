CC ?= cc
C_FLAGS := -Ofast -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Iinclude -Ideps/miniaudio -Ideps/termbox2

LD_FLAGS := -Lbuild -ldl -lpthread -lm

DIRECTORIES := build

INSTALL_DIRECTORY := /usr/local/bin

OBJECT_FILES := build/miniaudio.o build/termbox2.o $\
								build/audio.o build/directory.o build/global.o build/main.o build/menu.o

# setup
all: ${DIRECTORIES} tplayer

${DIRECTORIES}:
	-mkdir ${DIRECTORIES}

${OBJECT_FILES}: build/%.o :src/%.c
	${CC} -c $< ${C_FLAGS} -o $@

tplayer: build ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tplayer
	strip tplayer

# utils
clean:
	-rm -rf build
	-rm -f tplayer

install: all ${INSTALL_DIRECTORY}
	-cp -f tplayer ${INSTALL_DIRECTORY}
uninstall:
	-rm -f ${INSTALL_DIRECTORY}/tplayer

.PHONY: all clean install uninstall
