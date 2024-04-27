#include "Matrix.hpp"

using namespace algebra;

int main(){
    Matrix<int,Row> mat(3,3);

    std::cout << mat << std::endl;

    mat(0,0) = 0;
    mat(1,1) = 2;
    mat(2,2) = 3;

    mat.compress();

    std::vector<int> rhs{1,2,3};

    std::vector<int> res = mat*rhs;

    for(auto i : res){
        std::cout << i << " ";
    }


}