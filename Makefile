CXX = clang++
CXXFLAGS = -std=c++11
CXXFLAGS += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -I.
CXXFLAGS += -O2 -g -ggdb

.PHONY: all clean

all: example

example: d.o example.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.hh:

clean:
	rm -f *.o example
