#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <limits>

namespace algebra{

    enum StorageOrder {Row, Col};

    template <typename T, StorageOrder stor> class Matrix{
    public:
        Matrix(std::size_t size):size(size){};

        std::map<std::array<std::size_t,2>,T> data;
        std::size_t size;

        std::vector<T> val;
        std::vector<std::size_t> col_idx;
        std::vector<std::size_t> row_idx;

        T& operator()(std::size_t i, std::size_t j);
        T operator()(std::size_t i, std::size_t j) const;

        void resize(std::size_t newsize);

        void compress();
        void uncompress();
        bool is_compressed() const;

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
        if(!mat.is_compressed()){
            for(std::size_t i = 0; i<mat.size;++i){
                for(std::size_t j = 0; j<mat.size;++j){
                    str << mat(i,j) << " ";
                }
                str << std::endl;
            }
        }
        else{
            std::cout << "val: ";
            for(auto i : mat.val){
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "col_idx: ";
            for(auto i : mat.col_idx){
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "row_idx: ";
            for(auto i : mat.row_idx){
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        return str;

    }

    template<typename T, StorageOrder stor> void Matrix<T,stor>::resize(std::size_t newsize){
        if(!is_compressed()){
            if(newsize < size){
                for(std::size_t i = 0; i<size; ++i){
                    for(std::size_t j = 0; j<size; ++j){
                        if(i+j >= newsize){
                            data.erase({i,j});
                            #ifdef DEBUG
                            std::cout << "erased element " << i << "," <<j << std::endl;
                            #endif
                        }
                    }
                }
            }
            size = newsize;
        }
    }

    template<typename T, StorageOrder stor> void Matrix<T,stor>::compress(){
        if(!is_compressed()){
            row_idx.push_back(0);
            //this is so bad
            for(std::size_t i = 0; i<size;++i){
                std::size_t nnz_count = 0;
                for(std::size_t j = 0; j<size;++j){
                    if(this->operator()(i,j)!=0){
                        val.push_back(this->operator()(i,j));
                        col_idx.push_back(j);
                        nnz_count++;
                    }
                }
                row_idx.push_back(nnz_count + row_idx.back());
            }
            data.clear();
        }
    }

    template<typename T, StorageOrder stor> bool Matrix<T,stor>::is_compressed() const{
        return data.empty() && !val.empty(); //if matrix is empty (i.e. val is also empty), it's not compressed
    }

    template<typename T, StorageOrder stor> void Matrix<T,stor>::uncompress(){
        if(is_compressed()){
            int j=0;//row index for the matrix
            int i=0; //index for the vector of values
            int nnz_count = 0; //counts the non-zero elements currently inserted in the matrix
            while(i<val.size()){
                if(nnz_count >= row_idx[j+1]){ //if a row is filled, move to the next row
                    j++; 
                }
                else{ //otherwise, fill the row
                    this->operator()(j,col_idx[i]) = val[i];
                    nnz_count++;
                    i++;
                }
            }

            //clear all vectors

            val.clear();
            col_idx.clear();
            row_idx.clear();
        }
    }
}