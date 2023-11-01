CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++2a -g -O3

all: test search

run: search
	./search

test: test.cpp hyper_graph.cpp hyper_graph_family.cpp
	$(CXX) $(CXXFLAGS) test.cpp hyper_graph.cpp hyper_graph_family.cpp -o test

search: search.cpp hyper_graph.cpp hyper_graph_family.cpp
	$(CXX) $(CXXFLAGS) search.cpp hyper_graph.cpp hyper_graph_family.cpp -o search

clean:
	rm -f search
	rm -f test
	rm -f *.csv