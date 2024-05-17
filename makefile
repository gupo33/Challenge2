release: main.cpp include/Matrix.hpp include/MatrixRow.hpp include/MatrixCol.hpp include/MatrixRowCmpx.hpp include/MatrixColCmpx.hpp
	g++ -o main main.cpp  -O3 -std=c++20 

debug: main.cpp include/Matrix.hpp include/MatrixRow.hpp include/MatrixCol.hpp include/MatrixRowCmpx.hpp include/MatrixColCmpx.hpp
	g++ -o main main.cpp -g -std=c++20 -DDEBUG