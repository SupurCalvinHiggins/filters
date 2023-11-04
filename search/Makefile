CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -lpthread -std=c++2a -g -O3

all: test search

run: search
	./search

test: test.cpp hyper.h
	$(CXX) $(CXXFLAGS) test.cpp -o test

search: search.cpp hyper.h
	$(CXX) $(CXXFLAGS) search.cpp -o search

example: example.cpp hyper.h
	$(CXX) $(CXXFLAGS) example.cpp -o example
	./example

clean:
	rm -f search
	rm -f test
