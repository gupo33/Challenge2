#include <map>
#include <array>
#include <iostream>
#include <limits>

namespace algebra{

    enum StorageOrder {Row, Col};

    template <typename T, StorageOrder stor> class Matrix{
    public:
        Matrix(std::size_t size):size(size){};
        std::map<std::array<std::size_t,2>,T> data;
        std::size_t size;
        T& operator()(std::size_t i, std::size_t j);
        T operator()(std::size_t i, std::size_t j) const;
        void resize(std::size_t newsize);

    };

    template<typename T, StorageOrder stor> std::ostream& operator<<(std::ostream& str, const Matrix<T,stor>& mat);


    template<typename T, StorageOrder stor> T& Matrix<T,stor>::operator()(std::size_t i, std::size_t j){
        auto it = data.find({i,j});
        if(it!=data.end()){
            return it->second;
        }
        else{
            return data[{i,j}];
        }
    }

    template<typename T, StorageOrder stor> T Matrix<T,stor>::operator()(std::size_t i, std::size_t j) const{
        auto it = data.find({i,j});
        if(it!=data.end()){
            return it->second;
        }
        else{
           return 0;
        }
    }

    template<typename T, StorageOrder stor> std::ostream& operator<<(std::ostream& str, const Matrix<T,stor>& mat){
        for(std::size_t i = 0; i<mat.size;++i){
            for(std::size_t j = 0; j<mat.size;++j){
                str << mat(i,j) << " ";
            }
            str << std::endl;
        }

        return str;

    }

    template<typename T, StorageOrder stor> void Matrix<T,stor>::resize(std::size_t newsize){
        if(newsize < size){
            for(std::size_t i = 0; i<size; ++i){
                for(std::size_t j = 0; j<size; ++j){
                    if(i+j >= newsize){
                        data.erase({i,j});

                    }
                }
            }
        }
        size = newsize;
    }
}