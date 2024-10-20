CC ?= cc
C_FLAGS := -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Iinclude -Ideps/miniaudio -Ideps/termbox2

LD_FLAGS := -Lbuild -ldl -lpthread -lm

DIRECTORIES := build deps
DEPENDENCIES := deps/miniaudio deps/termbox2

INSTALL_DIRECTORY := /usr/local/bin

OBJECT_FILES := build/miniaudio.o build/termbox2.o $\
								build/audio.o build/directory.o build/global.o build/main.o build/menu.o

# setup
all: ${DIRECTORIES} ${DEPENDENCIES} tplayer

${DIRECTORIES}:
	-mkdir ${DIRECTORIES}

deps/miniaudio:
	git -C deps clone https://github.com/mackron/miniaudio --depth=1

deps/termbox2:
	git -C deps clone https://github.com/termbox/termbox2 --depth=1

${OBJECT_FILES}: build/%.o :src/%.c
	${CC} -c $< ${C_FLAGS} -o $@

tplayer: build ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tplayer
	strip tplayer

# utils
clean:
	-rm -rf build
	-rm -rf deps
	-rm -f tplayer

install: all ${INSTALL_DIRECTORY}
	-cp -f tplayer ${INSTALL_DIRECTORY}
uninstall:
	-rm -f ${INSTALL_DIRECTORY}/tplayer

.PHONY: all clean install uninstall
