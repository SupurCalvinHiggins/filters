CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++2a -g -O3

all: test search

run: search
	./search

test: clean test.cpp
	$(CXX) $(CXXFLAGS) test.cpp -o test
	./test

search: search.cpp
	$(CXX) $(CXXFLAGS) search.cpp -o search

clean:
	rm -f search
	rm -f test
	rm -f *.csv