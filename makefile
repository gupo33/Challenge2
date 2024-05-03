release: main.cpp include/MatrixRow.hpp include/MatrixCol.hpp
	g++ -o main main.cpp include/MatrixRow.hpp include/MatrixCol.hpp -O3 -std=c++20 

debug: main.cpp include/MatrixRow.hpp include/MatrixCol.hpp
	g++ -o main main.cpp include/MatrixRow.hpp include/MatrixCol.hpp -g -std=c++20 -DDEBUG