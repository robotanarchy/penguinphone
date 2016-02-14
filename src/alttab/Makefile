NAME=penguinphone-alttab
BIN=../../bin/$(NAME)
BIN_TESTSUITE=../../bin/$(NAME).testsuite
COMMON_OBJS=../../obj/common_wm_ipc.o
LIBS=`pkg-config --cflags --libs elementary`

$(BIN): *.c *.h $(COMMON_OBJS)
	$(CC) -o $@ *.c $(COMMON_OBJS) $(LIBS)



testsuite: $(BIN_TESTSUITE)
	$(VG) $(BIN_TESTSUITE)

$(BIN_TESTSUITE): *.c *.h testsuite/*
	$(CC) -o $@ testsuite/*.c $(COMMON_OBJS)






.PHONY: testsuite
