#include "MatrixCol.hpp"

using namespace algebra;

int main(){
    Matrix<int,Col> mat(3,3);

    mat(0,0) = 1;
    mat(1,1) = 2;
    mat(2,2) = 3;

    std::vector<int> a{1,2,3};

    std::cout << mat << std::endl;

    mat.compress();

    auto res = mat * a;

    for(auto i : res){
        std::cout << i << std::endl;
    }

}