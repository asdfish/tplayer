CC ?= cc
C_FLAGS := -Ofast -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Iinclude -Ideps/miniaudio -Ideps/termbox2

LD_FLAGS := -Lbuild -ldl -lpthread -lm

INSTALL_DIRECTORY := /usr/local/bin

OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))

# setup
all: tplayer

build/%.o: src/%.c
	${CC} -c $< ${C_FLAGS} -o $@

tplayer: ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tplayer
	strip tplayer

# utils
clean:
	$(foreach OBJECT_FILE,$\
		${OBJECT_FILES},$\
		$(if $(wildcard ${OBJECT_FILE}),$\
			$(shell rm ${OBJECT_FILE})))
ifneq (,$(wildcard tfm))
	rm tfm
endif

install: all ${INSTALL_DIRECTORY} uninstall
	cp tplayer ${INSTALL_DIRECTORY}

uninstall:
ifneq (,$(wildcard ${INSTALL_DIRECTORY}/tfm))
	rm ${INSTALL_DIRECTORY}/tfm
endif

.PHONY: all clean install uninstall
