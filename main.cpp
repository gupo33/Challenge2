#include "include/MatrixCol.hpp"
#include <chrono>

using namespace algebra;

int main(int argc, const char* argv[]){

    static std::chrono::_V2::system_clock::time_point t_start;
    Matrix<double,Row> mat;

    //read matrix from file

    mat.read(argv[1]);

    //create a right hand side

    std::vector<double> rhs;
    rhs.resize(131);

    for(int i = 0; i<rhs.size();++i){
        rhs[i] = i;
    }

    //perform matrix-vector product with the non-compressed matrix

    std::cout << "Performing matrix-vector product with non-compressed matrix" << std::endl;

    t_start = std::chrono::high_resolution_clock::now();

    std::vector<double> out = mat*rhs;

    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time is " << (t_end-t_start).count()*1E-9 << " seconds\n";

    mat.compress();

    //perform matrix-vector product with the compressed matrix

    std::cout << "Performing matrix-vector product with compressed matrix" << std::endl;

    t_start = std::chrono::high_resolution_clock::now();

    out = mat*rhs;

    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time is " << (t_end-t_start).count()*1E-9 << " seconds\n";
}