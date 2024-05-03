#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>

/*! \mainpage Matrix
 * A C++ library to handle sparse matrices. It allows to construct matrices of arbitrary size, accessing them, printing information about them, compressing,
 * uncompressing, performing matrix-vector multiplication, and reading the matrix from a file encoded in the Matrix-Market format.
 * The matrices can store any arbitrary data type, and can be ordered in row-major and column-major form.
 * The matrices, when uncompressed, are store in COOmap form, and can be compressed in CSR / CSC form.
 */

/// @file Matrix.hpp
/// @brief Contains the definition of the template class Matrix

//Alias for the matrix map key
using key = std::array<std::size_t,2>;

namespace algebra{
    /// @brief Enumerator indicating the ordering of the matrix
    enum StorageOrder {Row, Col};

    /// @brief Template Matrix class encoded in COOmap form, and compressible in CSR / CSC form, depending on the ordering
    /// @tparam T type of the elements of the Matrix
    /// @tparam stor Indicates how the Matrix is ordered (row-column major)
    template <typename T,StorageOrder stor> class Matrix;
}