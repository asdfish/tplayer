CC ?= cc
C_FLAGS := -Ofast -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Iinclude -Ideps/miniaudio -Ideps/termbox2

LD_FLAGS := -Lbuild -ldl -lpthread -lm

INSTALL_DIRECTORY := /usr/local/bin

PROCESSED_HEADER_FILES := $(subst .h,$\
														$(if $(findstring clang,${CC}),$\
															.h.pch,$\
															.h.gch),$\
														$(shell find include -name '*.h' -type f))

OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))

define REMOVE_LIST
	$(foreach ITEM,$\
		${1},$\
		$(if $(wildcard ${ITEM}),$\
			$(shell rm ${ITEM})))

endef

# setup
all: tplayer

build/%.o: src/%.c
	${CC} -c $< ${C_FLAGS} -o $@

%.gch: %
	${CC} -c $< ${C_FLAGS}

%.pch: %
	${CC} -c $< ${C_FLAGS}

tplayer: ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o tplayer
	strip tplayer

# utils
clean:
	$(call REMOVE_LIST,$\
		${PROCESSED_HEADER_FILES})
	$(call REMOVE_LIST,$\
		${OBJECT_FILES})
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
