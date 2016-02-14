CC:="LANG=C gcc -D_XOPEN_SOURCE=700 -std=c99 -g -Wall"
VG:="valgrind --track-origins=yes --leak-check=full"


all: common
	mkdir -p bin
	CC=$(CC) make -C src/status
	CC=$(CC) make -C src/alttab
	CC=$(CC) make -C src/launcher

common:
	mkdir -p obj
	CC=$(CC) make -C src/common

testenv: all
	./testenv.sh stop || true
	./testenv.sh start


testsuite: common
	mkdir -p bin
	VG=$(VG) CC=$(CC) make -C src/alttab testsuite
	VG=$(VG) CC=$(CC) make -C src/launcher testsuite
	VG=$(VG) CC=$(CC) make -C src/status testsuite


clean:
	rm -r bin obj || true


.PHONY: testenv all clean
