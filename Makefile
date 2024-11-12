CC ?= cc
C_FLAGS := -std=gnu11 $\
					 -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -I. -Iinclude -Ideps/miniaudio -Ideps/termbox2
LD_FLAGS := ${C_FLAGS} $\
						-ldl -lm -lpthread

OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))
PROCESSED_HEADER_FILES := $(subst .h,$\
														$(if $(findstring clang,${CC}),$\
															.h.pch,$\
															.h.gch),$\
														$(shell find include -name '*.h' -type f))

define COMPILE
$(info Compiling $(2))
@${CC} -c $(1) ${C_FLAGS} -o $(2)

endef
define REMOVE
$(if $(wildcard $(1)),$\
	$(info Removing $(1))
	$(shell rm $(1)))
endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(call REMOVE,${ITEM}))
endef

all: tplayer

tplayer: ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}
	$(info Linking $@)
	@${CC} ${OBJECT_FILES} ${LD_FLAGS} -o $@

build/%.o: src/%.c
	$(call COMPILE,$<,$@)

%.gch: %
	$(call COMPILE,$<,$@)

%.pch: %
	$(call COMPILE,$<,$@)

clean:
	$(call REMOVE,tplayer)
	$(call REMOVE_LIST,${OBJECT_FILES})
	$(call REMOVE_LIST,${PROCESSED_HEADER_FILES})

.PHONY: all clean
