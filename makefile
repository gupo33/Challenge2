release: main.cpp Matrix.hpp
	g++ -o main main.cpp Matrix.hpp -std=c++20 

debug: main.cpp Matrix.hpp
	g++ -o main main.cpp Matrix.hpp -g -std=c++20 -DDEBUG