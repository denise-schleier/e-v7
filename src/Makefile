WARNS = -W -Wall -pedantic -Wno-comment -Wno-variadic-macros -Wno-unused-function
V7_FLAGS = -DCS_ENABLE_UTF8
INCLUDES += -I../include
CFLAGS_EXTRA = -Wno-pedantic -Wno-implicit-fallthrough -Wno-unused-parameter
CFLAGS_PLATFORM = -DCS_PLATFORM=CS_P_UNIX
CFLAGS = $(WARNS) -g -O3 -lm $(INCLUDES) $(V7_FLAGS) $(CFLAGS_PLATFORM) $(CFLAGS_EXTRA)
TEST_CFLAGS = $(CFLAGS) -DV7_EXPOSE_PRIVATE -DV7_UNIT_TEST -DV7_FILENAMES_SUPPRESS_CFUNC_ADDR


all: v7 ev7 v7.js ev7.js


v7: v7.c ../include/v7.h Makefile
	$(CC) v7.c -o $@ -DV7_EXE $(CFLAGS) -lm

ev7: ev7.c v7.o ../include/v7.h Makefile
	$(CC) ev7.c v7.o -o $@ $(CFLAGS) -lm


v7.js: v7.c ../include/v7.h Makefile
	emcc v7.c -s WASM=1 -o $@ -DV7_EXE $(CFLAGS) -lm -pthread

ev7.js: ev7.c v7.c ../include/v7.h Makefile ../js/ev7_runtime.js
	emcc ev7.c v7.c -s WASM=1 -o $@ $(CFLAGS) -lm -pthread --preload-file ../js/ev7_runtime.js@ev7_runtime.js


clean:
	@$(MAKE) -C examples clean
	rm -f v7 ev7 v7.js ev7.js

v7.o: v7.c ../include/v7.h Makefile
	$(CC) -c v7.c -o $@ $(CFLAGS)
