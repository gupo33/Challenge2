#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>

/// @file MatrixRow.hpp
/// @brief Contains the implementation of the row-major matrix class

//Alias for the matrix map key
using key = std::array<std::size_t,2>;

namespace algebra{

    /// @brief Enumerator indicating the ordering of the matrix
    enum StorageOrder {Row, Col};

    //pre-declarations needed for template friends

    template <typename T,StorageOrder stor> class Matrix;
    template <typename T, StorageOrder stor> std::vector<T> operator*(const Matrix<T,stor>& lhs, const std::vector<T>& rhs);
    template <typename T, StorageOrder stor> std::ostream& operator<<(std::ostream& str, const Matrix<T,stor>& mat);

    /// @brief Template Matrix class encoded in COOmap form, and compressible in CSR / CSC form, depending on the ordering
    /// @tparam T type of the elements of the Matrix
    /// @tparam stor Indicates how the Matrix is ordered (row-column major)
    template <typename T,StorageOrder stor> class Matrix{ 
    private:
        /// @brief Container for the elements of the uncompressed Matrix, using the COOmap paradigm
        std::map<key,T> data;
        /// @brief Number of rows of the Matrix
        std::size_t num_row = 0;
        /// @brief Number of columns of the Matrix
        std::size_t num_col = 0;

        /// @brief Vector containing the non-zero elements of the compressed Matrix
        std::vector<T> val;
        /// @brief  Vector indicating the column indices of the non-zero elements of the compressed Matrix
        std::vector<std::size_t> col_idx;
        /// @brief Vector containing the number of non-zero elements in all previous rows for each row
        std::vector<std::size_t> row_idx;

    public:
        /// @brief Generates a Matrix with num_row * num_col size
        /// @param num_row Number of rows of the matrix
        /// @param num_col Number of columns of the matrix
        Matrix(std::size_t num_row, std::size_t num_col):num_row(num_row),num_col(num_col){};
        
        /// @brief Default constructor generating an empty Matrix
        Matrix() = default;

        /// @brief Read-Write access to an element of the Matrix, resizing if inserting out-of-bounds
        /// @param i Row index
        /// @param j Column index
        /// @return Reference to the element contained in the (i,j) cell of the Matrix
        T& operator()(std::size_t i, std::size_t j);

        /// @brief Read-only access to an element of the Matrix
        /// @param i row index
        /// @param j column index
        /// @return Const reference to element contained in the (i,j) cell of the Matrix        
        T operator()(std::size_t i, std::size_t j) const;

        /// @brief Resizes a Matrix, eventually deleting out-of-bounds non-zero elements
        /// @param new_row_num New number of rows 
        /// @param new_col_num New number of columns
        void resize(std::size_t new_row_num, std::size_t new_col_num);

        /// @brief Compresses an uncompressed Matrix following in CSR form, removing the uncompressed matrix from memory
        void compress();

        /// @brief Uncompresses a Matrix compressed in CSR form, removing the compressed matrix from memory
        void uncompress();
 
        /// @brief Checks if the Matrix is in the compressed state or not
        /// @return Bool indicating the compression status: true if compressed, false otherwise
        bool is_compressed() const;

        /// @brief Outputs the structure to the Matrix in the input stream, with different outputs depending on the compression status
        /// @param str stream to manipulate
        /// @param mat Matrix to output
        /// @return output stream
        friend std::ostream& operator<< <>(std::ostream& str, const Matrix<T,stor>& mat);

        /// @brief Performs Matrix-vector product between two correctly-dimensioned containers of the same type
        /// @param lhs Left hand side of the operation
        /// @param rhs Right hand side of the operation
        /// @return result vector
        friend std::vector<T> operator* <>(const Matrix<T,stor>& lhs, const std::vector<T>& rhs);

        /// @brief Clears an existing Matrix and fills it with the contents of a text file encoded in Matrix-Market format
        /// @param filename name of the file from which to read, positioned in the executable's directory 
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

    template<typename T, StorageOrder stor> void Matrix<T,stor>::resize(std::size_t new_row_num, std::size_t new_col_num){
        #ifdef DEBUG
            std::cout << "started resizing" << std::endl;
        #endif
        if(!is_compressed()){
            if(new_row_num < num_row || new_col_num < num_col){
                for(auto elem = data.begin(); elem != data.end();){
                    if(elem->first[0] >= new_row_num || elem->first[1] >= new_col_num){
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
            num_row = new_row_num;
            num_col = new_col_num;
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
        row_idx.resize(num_row+1);

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
                            std::cout << "val: " << std::endl;
                            for(auto v : val){
                                std::cout << v << " ";
                            }
                            std::cout << std::endl;
                            std::cout << "col_idx: " << std::endl;
                            for(auto v : col_idx){
                                std::cout << v << " ";
                            }
                            std::cout << std::endl;
                            std::cout << "row_idx: " << std::endl;
                            for(auto v : row_idx){
                                std::cout << v << " ";
                            }
                            std::cout << std::endl;
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
                    //read the dimensions from the first line with numbers
                    line_stream >> i;
                    line_stream >> j;
                    this->resize(i,j); //resize the matrix accordingly
                    #ifdef DEBUG
                        std::cout << "Dimensions: " <<i<<","<<j<<std::endl;
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