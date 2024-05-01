#include "MatrixCol.hpp"

using namespace algebra;

int main(int argc, const char* argv[]){
    Matrix<double,Row> mat;

    mat.read(argv[1]);

    std::cout << mat;

    mat.compress();

    std::cout << mat;

}