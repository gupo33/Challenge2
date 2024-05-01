release: main.cpp MatrixRow.hpp MatrixCol.hpp
	g++ -o main main.cpp MatrixRow.hpp MatrixCol.hpp -o3 -std=c++20 

debug: main.cpp MatrixRow.hpp MatrixCol.hpp
	g++ -o main main.cpp MatrixRow.hpp MatrixCol.hpp -g -std=c++20 -DDEBUG