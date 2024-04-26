#include "Matrix.hpp"
#include <limits>
#include <iostream>

namespace algebra{

    /*
    template<typename T, StorageOrder stor> const T& Matrix<T,stor>::operator()(std::size_t i, std::size_t j) const{
        if(i > size || j>size){
            std::cerr << "Out of bounds!" << std::endl;
            return std::numeric_limits<T>::quiet_NaN;
        }
        auto it = data.find({i,j});
        if(it!=data.end()){
            return it->second;
        }
        else{
           0;
        }
    }
    */

}