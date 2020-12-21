
CXXFLAGS = -std=c++11 -W -Wall -O3 -march=native -I../dsp
LDLIBS = -lfftw3
#LDLIBS = -lfftw3f

CXX = clang++ -stdlib=libc++
#CXX = g++
#CXX = x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++

test: benchmark
	./benchmark | tee data.txt

.PHONY: clean test

clean:
	rm -f benchmark

