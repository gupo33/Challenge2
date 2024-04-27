#include "Matrix.hpp"

using namespace algebra;

int main(){
    Matrix<double,Row> mat = Matrix<double,Row>(3,3);

    std::cout << mat << std::endl;

    mat.resize(5,2);

    std::cout << mat << std::endl;

    mat(0,0) = 1.5;
    mat(1,1) = 2.3;
    mat(3,1) = 5.4;
    mat(4,0) = 4.1;

    std::cout << mat << std::endl;

    mat(5,5) = 7.3;

    std::cout << mat << std::endl;

}