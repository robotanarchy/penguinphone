NAME=penguinphone-launcher
BIN=../../bin/$(NAME)
BIN_TESTSUITE=../../bin/$(NAME).testsuite
LIBS=`pkg-config --cflags --libs elementary`

$(BIN): *.c *.h
	$(CC) -o $@ *.c $(LIBS)


testsuite: $(BIN_TESTSUITE)
	cd testsuite && $(VG) ../$(BIN_TESTSUITE)

$(BIN_TESTSUITE): *.c *.h testsuite/*
	$(CC) -o $@ testsuite/*.c






.PHONY: testsuite
