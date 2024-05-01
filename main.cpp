#include "MatrixCol.hpp"

using namespace algebra;

int main(){
    Matrix<double,Row> mat;

    mat.read("matTest.txt");

    std::cout << mat;

    mat.compress();

    std::cout << mat;

}