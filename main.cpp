#include "Matrix.hpp"

using namespace algebra;

int main(){
    Matrix<int,Row> mat = Matrix<int,Row>(3,3);

    std::cout << mat << std::endl;

    mat.resize(5,2);

    std::cout << mat << std::endl;

    mat(0,0) = 1;
    mat(1,1) = 2;
    mat(3,1) = 5;
    mat(4,0) = 4;

    std::cout << mat << std::endl;

    mat.resize(2,2);

    std::cout << mat << std::endl;

}