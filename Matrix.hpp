#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>

/*! \mainpage Matrix
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */

namespace algebra{
    /// @brief Enumerator indicating the ordering of the matrix
    enum StorageOrder {Row, Col};
    
    template <typename T,StorageOrder stor> class Matrix;
}