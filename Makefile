
CXXFLAGS = -std=c++11 -W -Wall -O3 -march=native -I../dsp
LDLIBS = $(shell pkg-config fftw3 --libs)
#LDLIBS = -lfftw3
#LDLIBS = -lfftw3f

CXX = clang++ -stdlib=libc++
#CXX = g++

benchmark: benchmark.cc
	$(CXX) $(CXXFLAGS) $(LDLIBS) $< -o $@

test: benchmark
	./benchmark

.PHONY: clean test

clean:
	rm -f benchmark

