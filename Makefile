CC:="LANG=C gcc -D_XOPEN_SOURCE=700 -std=c99 -g -Wall"
VG:="valgrind --track-origins=yes --leak-check=full"


all: common
	mkdir -p bin
	CC=$(CC) $(MAKE) -C src/status
	CC=$(CC) $(MAKE) -C src/alttab
	CC=$(CC) $(MAKE) -C src/launcher

common:
	mkdir -p obj
	CC=$(CC) $(MAKE) -C src/common

testenv: all
	./testenv.sh stop || true
	./testenv.sh start


testsuite: common
	mkdir -p bin
	VG=$(VG) CC=$(CC) $(MAKE) -C src/alttab testsuite
	VG=$(VG) CC=$(CC) $(MAKE) -C src/launcher testsuite
	VG=$(VG) CC=$(CC) $(MAKE) -C src/status testsuite


clean:
	rm -r bin obj || true


.PHONY: testenv all clean
