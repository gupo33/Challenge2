release: main.cpp include/Matrix.hpp include/MatrixRow.hpp include/MatrixCol.hpp include/MatrixRowCmpx.hpp include/MatrixColCmpx.hpp
	g++ -o main main.cpp include/MatrixColCmpx.hpp -O3 -std=c++20 

debug: main.cpp include/MatrixRow.hpp include/MatrixCol.hpp include/MatrixRowCmpx.hpp include/MatrixColCmpx.hpp
	g++ -o main main.cpp include/MatrixColCmpx.hpp -g -std=c++20 -DDEBUG