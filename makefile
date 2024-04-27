release: main.cpp Matrix.cpp Matrix.hpp
	g++ -o main main.cpp Matrix.cpp Matrix.hpp -g -std=c++20 

debug: main.cpp Matrix.cpp Matrix.hpp
	g++ -o main main.cpp Matrix.cpp Matrix.hpp -g -std=c++20 -DDEBUG