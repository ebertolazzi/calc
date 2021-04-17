INCDIR =
LIBS   = -lm
CFLAGS =

all:
	@echo "To compile test files do:"
	@echo ""
	@echo "\"make cw\"  for OSX Codewarrior compiler"
	@echo "\"make gcc\" for gnu (g++) compiler"
	@echo "\"make cxx\" for digital cxx compiler"
	@echo "\"make kcc\" for KCC compiler"
	@echo ""
	@echo "To clean up the directory do:"
	@echo ""
	@echo "\`\`make clean''"
	@echo ""

GCCF="-g0 -O -ansi"

CCF="-g0 -O -ansi -Wno-long-double -DUSE_OLD_STRSTREAM"

CWF="-O3 -Op -inline deferred -ansi strict -proto strict  -msgstyle std"


cw:
	make CC=mwccppc LIBS="" CFLAGS=${CWF} compile

cxx:
	make CC=cxx CFLAGS="-w0 -g0 -O -om -std strict_ansi" compile

gcc:
	make CC=g++ CFLAGS=${GCCF} compile

kcc:
	make CC=KCC CFLAGS="-g -O --strict" compile

compile:
	$(CC) $(CFLAGS) -Isrc tests/calc_test.cc -o tests/calc_test $(LIBS)
	$(CC) $(CFLAGS) -Isrc tests/testall.cc   -o tests/testall   $(LIBS)

clean:
	rm -f calc *~ pch/calcPPC++ calcPPC.*
	rm -rf "calcPPC Data"