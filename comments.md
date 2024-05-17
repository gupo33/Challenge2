in the code prefixed with @note

The matrix times vector for compressed matrices is inefficient. You have to iterate over the outer index and then the inner index. For a row-major matrix, you do
```cpp
for (std::size_t i = 0; i < rows; ++i) {
    for (auto k = outer[i]; k < outer[i + 1]; ++k) {
        result[i] += values[k] * vector[inner[k]];
    }
}
```
for a column-major matrix, you do
```cpp
for (std::size_t j = 0; j < cols; ++j) {
    for (auto k = outer[j]; k < outer[j + 1]; ++k) {
        result[inner[k]] += values[k] * vector[j];
    }
}
```
If you have a method that works only if the matrix is compressed/uncompressed it is better  write an error message
or throw an exeption if used uncorrectly. 

Other comemnts in the code


