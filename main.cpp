//@note you normally indicate the include directories in the compiler option not
// in the code
#include "include/MatrixColCmpx.hpp"
#include <chrono>
// clang-format off

using namespace algebra;

int main(int argc, const char* argv[]){

    static std::chrono::_V2::system_clock::time_point t_start;
    Matrix<double,Row> mat;

    //read matrix from file
//@note dont do like that. Use getpot to parse the command line arguments
// and print a help for the -h option. Otherwise the users does not know what to do.
// Also becouse you have not written how to launch the code in the README.md file!
    mat.read(argv[1]);

    //create a right hand side

    std::vector<double> rhs;
    rhs.resize(131);

    for(int i = 0; i<rhs.size();++i){
        rhs[i] = i;
    }

    std::cout << "Testing row-major matrix" << std::endl;

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

    std::cout << "Testing column-major matrix" << std::endl;

    Matrix<double,Col> mat2;

    mat2.read(argv[1]);

    std::cout << "Performing matrix-vector product with non-compressed matrix" << std::endl;

    t_start = std::chrono::high_resolution_clock::now();

    std::vector<double> out2 = mat*rhs;

    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time is " << (t_end-t_start).count()*1E-9 << " seconds\n";

    mat.compress();

    //perform matrix-vector product with the compressed matrix

    std::cout << "Performing matrix-vector product with compressed matrix" << std::endl;

    t_start = std::chrono::high_resolution_clock::now();

    out = mat*rhs;

    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time is " << (t_end-t_start).count()*1E-9 << " seconds\n";

    return 0;
}