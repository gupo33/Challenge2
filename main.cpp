#include "Matrix.hpp"

using namespace algebra;

int main(){
    Matrix<int,Row> mat(3,3);

    mat(2,2) = 1;
    mat(1,2) = 1;
    mat(2,1) = 1;

    std::cout << mat << std::endl;

    mat.compress();

    std::cout << mat << std::endl;

}