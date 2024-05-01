#include "MatrixRow.hpp"

#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <limits>

//Column-major specialization of the matrix class

namespace algebra{

    //pre-declarations needed for template friends

    template <typename T> class Matrix<T,Col>;
    template <typename T> std::vector<T> operator*(const Matrix<T,Col>& lhs, const std::vector<T>& rhs);
    template <typename T> std::ostream& operator<<(std::ostream& str, const Matrix<T,Col>& mat);

    struct cmpCol{ //custom comparator for column-major matrices
        bool operator()(const key& lhs, const key& rhs) const{
            return(lhs[1] < rhs[1] || (lhs[1] == rhs[1] && lhs[0] < rhs[0]));
        }
    };

    template <typename T> class Matrix<T,Col>{ 
    private:
        std::map<key,T,cmpCol> data;
        std::size_t row_size;
        std::size_t col_size;

        std::vector<T> val;
        std::vector<std::size_t> col_idx;
        std::vector<std::size_t> row_idx;

    public:
        Matrix(std::size_t row_size, std::size_t col_size):row_size(row_size),col_size(col_size){};

        T& operator()(std::size_t i, std::size_t j);
        T operator()(std::size_t i, std::size_t j) const;

        void resize(std::size_t row_newsize, std::size_t col_newsize);

        void compress();
        void uncompress();
        bool is_compressed() const;

        friend std::ostream& operator<< <>(std::ostream& str, const Matrix<T,Col>& mat);
        friend std::vector<T> operator* <>(const Matrix<T,Col>& lhs, const std::vector<T>& rhs);

    };

    template<typename T> T& Matrix<T,Col>::operator()(std::size_t i, std::size_t j){
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

    template<typename T> T Matrix<T,Col>::operator()(std::size_t i, std::size_t j) const{
        auto it = data.find({i,j});
        if(it!=data.end()){
            return it->second;
        }
        else{
           return 0;
        }
    }

    template<typename T> std::ostream& operator<<(std::ostream& str, const Matrix<T,Col>& mat){
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

            std::cout << "row_idx: ";
            for(auto i : mat.row_idx){
                std::cout << i << " ";
            }
            std::cout << std::endl;

            std::cout << "col_idx: ";
            for(auto i : mat.col_idx){
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        return str;

    }

    template<typename T> void Matrix<T,Col>::resize(std::size_t row_newsize, std::size_t col_newsize){
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

    template<typename T> void Matrix<T,Col>::compress(){
        #ifdef DEBUG
            std::cout << "started compressing" << std::endl;
        #endif

        //initialize vectors
        col_idx.resize(row_size+1);

        if(!is_compressed()){
           for(std::size_t i = 0; i<col_size;++i){

                col_idx[i+1]+=col_idx[i];

                for(auto elem = data.lower_bound({0,i}); elem!= data.upper_bound({row_size,i}); ++elem){
                    if(elem->second!=0){
                        val.push_back(elem->second);
                        row_idx.push_back(elem->first[0]);
                        col_idx[i+1]++;
                        #ifdef DEBUG
                            std::cout << "inserted element in position " << elem->first[0] <<","<<i <<std::endl;
                        #endif
                    }
                }
           }
            data.clear();
        }
        #ifdef DEBUG
            std::cout << "finished compressing" << std::endl;
        #endif
    }

    template<typename T> bool Matrix<T,Col>::is_compressed() const{
        return data.empty() && !val.empty(); //if matrix is empty (i.e. val is also empty), it's not compressed
    }

    template<typename T> void Matrix<T,Col>::uncompress(){
        if(is_compressed()){
            int j=0;//column index for the matrix
            int i=0; //index for the vector of values
            int nnz_count = 0; //counts the non-zero elements currently inserted in the matrix
            while(i<val.size()){
                if(nnz_count >= col_idx[j+1]){ //if a column is filled, move to the next column
                    j++; 
                }
                else{ //otherwise, fill the column
                    this->operator()(row_idx[i],j) = val[i];
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

    template<typename T> std::vector<T> operator*(const Matrix<T,Col>& lhs, const std::vector<T>& rhs){
        //assuming that the dimensions make sense...
        std::vector<T> result;
        result.resize(lhs.row_size);
        if(!lhs.is_compressed()){
            for(std::size_t i = 0; i<lhs.col_size; ++i){
                for(std::size_t j = 0; j<lhs.row_size; ++j){
                    result[i] += lhs(j,i)*rhs[j];   
                }
            }
        }
        else{
            int j=0;//column index for the matrix
            int i=0; //index for the vector of values
            int nnz_count = 0; //counts the non-zero elements currently inserted in the matrix
            while(i<lhs.val.size()){
                if(nnz_count >= lhs.col_idx[j+1]){ //if a column is filled, move to the next column
                    j++; 
                }
                else{ //otherwise, fill the row
                    result[j]+=lhs.val[i]*rhs[lhs.row_idx[i]];
                    nnz_count++;
                    i++;
                }
            }
        }
        return(result);
    }

}