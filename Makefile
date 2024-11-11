CC ?= cc
C_FLAGS := -std=c99 $\
					 -O2 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -I. -Iinclude -Ideps/miniaudio -Ideps/termbox2

OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c'))

define REMOVE
$(if $(wildcard $(1)),$\
	$(info Removing $(1))
	$(shell rm $(1)))
endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(call REMOVE,${ITEM}))
endef

all: cplayer

cplayer: ${OBJECT_FILES}
	$(info Linking $@)
	@${CC} $< ${C_FLAGS} -o $@

build/%.o: src/%.c
	$(info Compiling $@)
	@${CC} -c $< ${C_FLAGS} -o $@

clean:
	$(call REMOVE,cplayer)
	$(call REMOVE_LIST,${OBJECT_FILES})

.PHONY: all clean
