#include "MatrixCol.hpp"

using namespace algebra;

int main(){
    Matrix<int,Col> mat(4,3);

    mat(0,0) = 1;
    mat(0,2) = 2;
    mat(1,1) = 2;
    mat(3,2) = 3;

    std::vector<int> a{1,2,3};

    std::cout << mat << std::endl;

    mat.compress();

    std::vector<int> out = mat*a;

    for(auto v : out){
        std::cout << v << " ";
    }

}