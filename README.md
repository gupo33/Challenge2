# C++ library for sparse matrices

This repository contains a library built to handle sparse elements with generic content, both row and column major. More information is available in the Doxygen documentation. 


## Contents

- The `include` folder contains the header files with the implementation.
- `main.cpp` contains an example main that handles reading a matrix from a file and performing matrix-vector product with a correctly-dimensioned vector, while checking the performance differences between the compressed and uncompressed matrices
## Compilation instructions

```bash 
make release 
``` 
Generates the main file to be ran, requiring the name of the file from which to read the matrix when called.
