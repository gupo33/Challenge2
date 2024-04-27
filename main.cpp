#include "Matrix.hpp"

using namespace algebra;

int main(){
    Matrix<int,Row> mat = Matrix<int,Row>(3);

    std::cout << mat << std::endl;

    mat.resize(4);

    std::cout << mat << std::endl;

    mat(3,3) = 4;

    std::cout << mat << std::endl;

    mat.resize(3);

    std::cout << mat << std::endl;

    std::cout << mat(3,3);

}