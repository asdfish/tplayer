CC ?= cc
C_FLAGS := -std=gnu11 $\
					 -Og -g -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -Iinclude $\
					 -Ideps/miniaudio -Ideps/orchestra/include -Ideps/tb_menu/include -Ideps/termbox2
LD_FLAGS := ${C_FLAGS} $\
						-ldl -lm -lpthread $\
						-Ldeps/orchestra -lorchestra $\
						-Ldeps/tb_menu -ltb_menu

MAKE ?= make

OBJECT_FILES := $(patsubst src/%.c,$\
									build/%.o,$\
									$(shell find src -name '*.c' -type f))

# Uncomment to enable processing header files
PROCESS_HEADER_FILES := yes
PROCESSED_HEADER_FILES := $(if ${PROCESS_HEADER_FILES},$\
														$(subst .h,$\
															$(if $(findstring clang,${CC}),$\
																.h.pch,$\
																.h.gch),$\
															$(shell find -name '*.h' -type f -not -path './deps/*')))

TPLAYER_REQUIREMENTS := deps/orchestra/liborchestra.a deps/tb_menu/libtb_menu.a ${PROCESSED_HEADER_FILES} ${OBJECT_FILES}

define COMPILE
$(info Compiling $(2))
@${CC} -c $(1) ${C_FLAGS} -o $(2)

endef
define REMOVE
$(if $(wildcard $(1)),$\
	$(info Removing $(1))
	$(shell rm $(1)))
endef
define REMOVE_DEPENDENCY
$(if $(wildcard $(1)),$\
	$(MAKE) -C $(dir $(1)) clean)

endef
define REMOVE_LIST
$(foreach ITEM,$\
	$(1),$\
	$(if $(findstring .a,${ITEM}),$\
		$(call REMOVE_DEPENDENCY,${ITEM}),$\
		$(call REMOVE,${ITEM})))
endef

all: tplayer

tplayer: ${TPLAYER_REQUIREMENTS}
	$(info Linking $@)
	@${CC} ${OBJECT_FILES} ${LD_FLAGS} -o $@

build/%.o: src/%.c
	$(call COMPILE,$<,$@)

%.gch: %
	$(call COMPILE,$<,$@)

%.pch: %
	$(call COMPILE,$<,$@)

%.a:
	$(MAKE) -C $(dir $@)

clean:
	$(call REMOVE_LIST,${TPLAYER_REQUIREMENTS})

.PHONY: all clean
