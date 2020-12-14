
CXXFLAGS = -std=c++11 -W -Wall -O3 -march=native -I../dsp
LIBS = $(shell pkg-config fftw3 --libs)

CXX = clang++ -stdlib=libc++
#CXX = g++

benchmark: benchmark.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $< -o $@

test: benchmark
	./benchmark

.PHONY: clean test

clean:
	rm -f benchmark

