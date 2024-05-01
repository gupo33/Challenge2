#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>

using key = std::array<std::size_t,2>; //Alias for the matrix map key

namespace algebra{

    enum StorageOrder {Row, Col};

    //pre-declarations needed for template friends

    template <typename T,StorageOrder stor> class Matrix;
    template <typename T, StorageOrder stor> std::vector<T> operator*(const Matrix<T,stor>& lhs, const std::vector<T>& rhs);
    template <typename T, StorageOrder stor> std::ostream& operator<<(std::ostream& str, const Matrix<T,stor>& mat);



    template <typename T,StorageOrder stor> class Matrix{ 
    private:
        std::map<key,T> data;
        std::size_t num_row = 0;
        std::size_t num_col = 0;

        std::vector<T> val;
        std::vector<std::size_t> col_idx;
        std::vector<std::size_t> row_idx;

    public:
        Matrix(std::size_t num_row, std::size_t num_col):num_row(num_row),num_col(num_col){};
        Matrix() = default;

        T& operator()(std::size_t i, std::size_t j);
        T operator()(std::size_t i, std::size_t j) const;

        void resize(std::size_t row_newsize, std::size_t col_newsize);

        void compress();
        void uncompress();
        bool is_compressed() const;

        friend std::ostream& operator<< <>(std::ostream& str, const Matrix<T,stor>& mat);
        friend std::vector<T> operator* <>(const Matrix<T,stor>& lhs, const std::vector<T>& rhs);

        void read(std::string filename);

    };

    template<typename T, StorageOrder stor> T& Matrix<T,stor>::operator()(std::size_t i, std::size_t j){
        auto it = data.find({i,j});
        if(it!=data.end()){
            return it->second;
        }
        else{
            if(i>=num_row || j>=num_col){ //resize the matrix if out of bounds
                resize(std::max(num_row,i+1),std::max(num_col,j+1));
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
            for(std::size_t i = 0; i<mat.num_row;++i){
                for(std::size_t j = 0; j<mat.num_col;++j){
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
            if(row_newsize < num_row || col_newsize < num_col){
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
            num_row = row_newsize;
            num_col = col_newsize;
            #ifdef DEBUG
                std::cout << "finished resizing" << std::endl;
            #endif
        }
    }

    template<typename T, StorageOrder stor> void Matrix<T,stor>::compress(){
        #ifdef DEBUG
            std::cout << "started compressing" << std::endl;
        #endif

        //initialize vectors
        row_idx.resize(num_col+1);

        if(!is_compressed()){
           for(std::size_t i = 0; i<num_row;++i){

                row_idx[i+1]+=row_idx[i];

                for(auto elem = data.lower_bound({i,0}); elem!= data.upper_bound({i,num_col}); ++elem){
                    if(elem->second!=0){
                        val.push_back(elem->second);
                        col_idx.push_back(elem->first[1]);
                        row_idx[i+1]++;
                        #ifdef DEBUG
                            std::cout << "inserted element in position " << i <<","<<elem->first[1] <<std::endl;
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
        std::vector<T> result;
        result.resize(lhs.num_row);
        if(!lhs.is_compressed()){
            for(std::size_t i = 0; i<lhs.num_row; ++i){
                for(std::size_t j = 0; j<lhs.num_col; ++j){
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

    template<typename T, StorageOrder stor> void Matrix<T,stor>::read(std::string filename){

        this->data.clear(); //clears what was contained previously in the matrix

        std::ifstream file{filename}; //file stream
        std::string line; //contains each parsed line
        bool startFlag = false; //checks if the matrix data has been reached yet
        std::size_t i,j;
        T val;
        while(file){

            std::getline(file,line);

            if(!startFlag){
                if(line[0]!='%'){ //skips initial comments
                    startFlag = true; //signals start of the data
                    std::istringstream line_stream(line);
                    std::array<std::size_t,2> dims; //read the dimensions from the first line with numbers
                    line_stream >> dims[0];
                    line_stream >> dims[1];
                    this->resize(dims[0],dims[1]);
                    #ifdef DEBUG
                        std::cout << "Dimensions: " <<dims[0]<<","<<dims[1]<<std::endl;
                    #endif
                }
            }
            else{
                std::istringstream line_stream(line); //reads each line and stores each element inside the matrix
                #ifdef DEBUG
                    std::cout << "Current line: " << line << std::endl;
                #endif
                line_stream >> i;
                line_stream >> j;
                line_stream >> val;
                this->data[{i-1,j-1}] = val; //coordinates in matrix market format start from one
            }
        }
    }
}