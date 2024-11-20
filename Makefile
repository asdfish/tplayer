define ENABLE_CPU_OPTIMIZATION
$(if $(shell grep -o $(1) /proc/cpuinfo),$\
	-m$(1),$\
	-DMA_NO_$(2))
endef

CC ?= cc
C_FLAGS := -std=gnu11 $\
					 -O3 -march=native -pipe $\
					 -Wall -Wextra -Wpedantic $\
					 -Wno-missing-field-initializers -Wno-unused-parameter $\
					 -Iinclude $\
					 $(call ENABLE_CPU_OPTIMIZATION,avx,AVX) $\
					 $(call ENABLE_CPU_OPTIMIZATION,sse2,SSE2) $\
					 $(call ENABLE_CPU_OPTIMIZATION,neon,NEON) $\
					 -Ideps/miniaudio -Ideps/orchestra/include -Ideps/tb_menu/include -Ideps/termbox2
LD_FLAGS := ${C_FLAGS} $\
						-ldl -lm -lpthread $\
						-Ldeps/orchestra -lorchestra $\
						-Ldeps/tb_menu -ltb_menu

MAKE ?= make

INSTALL_DIRECTORY := /usr/local/bin

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
${CC} -c $(1) ${C_FLAGS} -o $(2)

endef
define REMOVE
$(if $(wildcard $(1)),$\
	rm $(1))

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
	${CC} ${OBJECT_FILES} ${LD_FLAGS} -o $@
	strip $@

build/%.o: src/%.c
	$(call COMPILE,$<,$@)

%.gch: %
	$(call COMPILE,$<,$@)

%.pch: %
	$(call COMPILE,$<,$@)

%.a:
	CFLAGS='${C_FLAGS}' $(MAKE) -C $(dir $@)

clean:
	$(call REMOVE_LIST,${TPLAYER_REQUIREMENTS})
	$(call REMOVE,tplayer)

install: tplayer ${INSTALL_DIRECTORY} uninstall
	cp tplayer ${INSTALL_DIRECTORY}

uninstall:
ifneq (,$(wildcard ${INSTALL_DIRECTORY}/tplayer))
	rm ${INSTALL_DIRECTORY}/tplayer
endif

.PHONY: all clean install uninstall
