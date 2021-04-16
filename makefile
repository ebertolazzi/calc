LIBS="-lm"

GCCF="-g0 -O -Wall" 
CXXF="-w0 -g0 -O -std strict_ansi"
KCCF="-g -O --strict"

cc:
	make CXX="c++ -std=c++11" CXXFLAGS=${GCCF} all

gcc:
	make CXX="g++ -std=c++11" CXXFLAGS=${GCCF} all

all:
	$(CXX) calc_test.cc -o t $(LIBS)

clean:
	rm -f t
