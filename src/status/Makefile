NAME=penguinphone-status
BIN=../../bin/$(NAME)
BIN_TESTSUITE=../../bin/$(NAME).testsuite
COMMON_OBJS=../../obj/common_wm_ipc.o


$(BIN): *.c *.h $(COMMON_OBJS)
	$(CC) -o $@ *.c $(COMMON_OBJS)



testsuite: $(BIN_TESTSUITE)
	$(VG) $(BIN_TESTSUITE)

$(BIN_TESTSUITE): *.c *.h testsuite/*
	$(CC) -o $@ testsuite/*.c $(COMMON_OBJS)






.PHONY: testsuite
