#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <limits>

namespace algebra{

    enum StorageOrder {Row, Col};

    template <typename T, StorageOrder stor> class Matrix{
    public:
        Matrix(std::size_t row_size, std::size_t col_size):row_size(row_size),col_size(col_size){};

        std::map<std::array<std::size_t,2>,T> data;
        std::size_t row_size;
        std::size_t col_size;

        std::vector<T> val;
        std::vector<std::size_t> col_idx;
        std::vector<std::size_t> row_idx;

        T& operator()(std::size_t i, std::size_t j);
        T operator()(std::size_t i, std::size_t j) const;

        void resize(std::size_t row_newsize, std::size_t col_newsize);

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
            if(i>=row_size || j>=col_size){ //resize the matrix if out of bounds
                resize(std::max(row_size,i+1),std::max(col_size,j+1));
            }
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
            for(std::size_t i = 0; i<mat.row_size;++i){
                for(std::size_t j = 0; j<mat.col_size;++j){
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

    template<typename T, StorageOrder stor> void Matrix<T,stor>::resize(std::size_t row_newsize, std::size_t col_newsize){
        #ifdef DEBUG
            std::cout << "started resizing" << std::endl;
        #endif
        if(!is_compressed()){
            if(row_newsize < row_size || col_newsize < col_size){
                for(auto elem = data.begin(); elem != data.end();){
                    if(elem->first[0] >= row_newsize || elem->first[1] >= col_newsize){
                        #ifdef DEBUG
                            std::cout << "erased element in position" << elem->first[0] << "," << elem->first[1] << std::endl;
                        #endif
                        data.erase(elem++); //remove the element pointed by elem and move to the next one to keep the pointer valid
                    }
                    else{
                        elem++;
                    }
                }
            }
            row_size = row_newsize;
            col_size = col_newsize;
            #ifdef DEBUG
                std::cout << "finished resizing" << std::endl;
            #endif
        }
    }

    template<typename T, StorageOrder stor> void Matrix<T,stor>::compress(){
        if(!is_compressed()){
            row_idx.push_back(0);
            //TODO: this is so bad
            for(std::size_t i = 0; i<row_size;++i){
                std::size_t nnz_count = 0;
                for(std::size_t j = 0; j<col_size;++j){
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

    template<typename T, StorageOrder stor> std::vector<T> operator*(const Matrix<T,stor>& lhs, const std::vector<T>& rhs){
        //assuming that the dimensions make sense...
        std::vector<T> result;
        result.resize(lhs.col_size);
        if(!lhs.is_compressed()){
            for(std::size_t i = 0; i<lhs.row_size; ++i){
                for(std::size_t j = 0; j<lhs.col_size; ++j){
                    result[i] += lhs(i,j)*rhs[j];   
                }
            }
        }
        else{
            int j=0;//row index for the matrix
            int i=0; //index for the vector of values
            int nnz_count = 0; //counts the non-zero elements currently inserted in the matrix
            while(i<lhs.val.size()){
                if(nnz_count >= lhs.row_idx[j+1]){ //if a row is filled, move to the next row
                    j++; 
                }
                else{ //otherwise, fill the row
                    result[j]+=lhs.val[i]*rhs[lhs.col_idx[i]];
                    nnz_count++;
                    i++;
                }
            }
        }
        return(result);
    }
}