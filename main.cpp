#include "Matrix.hpp"

using namespace algebra;

int main(){
    Matrix<int,Row> mat = Matrix<int,Row>(3);

    std::cout << mat << std::endl;

    mat(0,0) = 1;
    mat(0,1) = 2;
    mat(2,2) = 3;

    std::cout << mat << std::endl;

    std::cout << mat.is_compressed() << std::endl;

    mat.compress();

    std::cout << mat.is_compressed() << std::endl;

    std::cout << mat << std::endl;

    mat.uncompress();

    std::cout << mat << std::endl;

    std::cout << mat.is_compressed();

    mat.compress();

    std::cout << mat << std::endl;

}