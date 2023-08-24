WARNS = -W -Wall -pedantic -Wno-comment -Wno-variadic-macros -Wno-unused-function
V7_FLAGS = -DCS_ENABLE_UTF8
CFLAGS_PLATFORM = -DCS_PLATFORM=CS_P_UNIX
CFLAGS = $(WARNS) -g -O3 -lm $(V7_FLAGS) $(CFLAGS_PLATFORM) $(CFLAGS_EXTRA)
TEST_CFLAGS = $(CFLAGS) -DV7_EXPOSE_PRIVATE -DV7_UNIT_TEST -DV7_FILENAMES_SUPPRESS_CFUNC_ADDR

.PHONY: examples test

all: v7 examples test

examples:
	@$(MAKE) -C examples

tests/unit_test: tests/unit_test.c tests/common/test_util.c tests/common/test_util.h v7.c v7.h Makefile
		$(CC) tests/unit_test.c tests/common/test_util.c $(TEST_CFLAGS) -lm -o tests/unit_test

test: tests/unit_test
	cd tests; TZ=UTC ./unit_test $(TEST_FILTER)

v7: v7.c v7.h Makefile
	$(CC) v7.c -o $@ -DV7_EXE $(CFLAGS) -lm

v7.js: v7.c v7.h Makefile
	emcc v7.c -s WASM=1 -o $@ -DV7_EXE $(CFLAGS) -lm -pthread

ev7: ev7.c v7.o v7.h Makefile
	$(CC) ev7.c v7.o -o $@ $(CFLAGS) -lm

ev7.js: ev7.c v7.c v7.h Makefile ev7_runtime.js
	emcc ev7.c v7.c -s WASM=1 -o $@ $(CFLAGS) -lm -pthread --preload-file ev7_runtime.js

v7.o: v7.c v7.h Makefile
	$(CC) -c v7.c -o $@ $(CFLAGS)

clean:
	@$(MAKE) -C examples clean
	rm -fr v7 v7.dSYM tests/unit_test
