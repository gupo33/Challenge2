#include "Matrix.hpp"

using namespace algebra;

int main(){
    Matrix<int,Row> mat = Matrix<int,Row>(3);

    std::cout << mat << std::endl;

}