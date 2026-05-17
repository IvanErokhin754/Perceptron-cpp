#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <stdexcept>
#include <cmath>

template <typename T>

class Matrix {
private:
    size_t size_n, size_m;
    std::vector<T> data;

    size_t Index(size_t i, size_t j) const {
        return i * size_m + j;
    }

    void CheckBounds(size_t i, size_t j) const {
        if (i >= size_n || j >= size_m)
            throw std::out_of_range("Index out of range");
    }

public:
    Matrix() : size_n(0), size_m(0), data() {}
    Matrix(size_t n, size_t m, const T& default_value = T()) : size_n(n), size_m(m), data(n * m, default_value) {}
    Matrix(const Matrix<T>& other) : size_n(other.size_n), size_m(other.size_m), data(other.data) {}

    Matrix<T>& operator=(const Matrix<T>& other) {
        if (this == &other)
            return *this; 

        size_n = other.size_n;
        size_m = other.size_m;
        data = other.data;
        return *this;
    }

    size_t GetRows() const {
        return size_n;
    }
    
    size_t GetCols() const {
        return size_m;
    }

    Matrix<T> operator+(const Matrix<T>& other) const {
        return Add(other);
    }

    Matrix<T> operator*(const T& scalar) const {
        return MultiplyByScalar(scalar);
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        return Multiply(other);
    }

    bool operator==(const Matrix<T>& other) const {
        if (size_n != other.size_n || size_m != other.size_m)
            return false;
        
        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < size_m; j++) {
                if (Get(i, j) != other.Get(i, j))
                    return false;
            }
        }

        return true;
    }

    bool operator!=(const Matrix<T>& other) const {
        return !(*this == other);
    }

    T& operator()(size_t i, size_t j) {
        CheckBounds(i, j);
        return data[Index(i, j)];
    }

    const T& operator()(size_t i, size_t j) const {
        CheckBounds(i, j);
        return data[Index(i, j)];
    }

    const T& Get(size_t i, size_t j) const {
        return (*this)(i, j);
    }

    void Set(size_t i, size_t j, const T& value) {
        (*this)(i, j) = value;
    }  
    
    Matrix<T> Add(const Matrix<T>& other) const {
        if (other.size_n != size_n || other.size_m != size_m)
            throw std::invalid_argument("Matrix sizes must match");
        
        Matrix<T> result(size_n, size_m);

        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < size_m; j++) {
                result.Set(i, j, Get(i, j) + other.Get(i, j));
            }       
        }

        return result;
    }

    Matrix<T> MultiplyByScalar(const T& scalar) const {
        Matrix<T> result(size_n, size_m);

        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < size_m; j++) {
                result.Set(i, j, Get(i, j) * scalar);
            }
        }

        return result;
    }

    Matrix<T> Multiply(const Matrix<T>& other) const {
        if (size_m != other.size_n)
            throw std::invalid_argument("Matrix sizes must match");
        
        Matrix<T> result(size_n, other.size_m, T());

        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < other.size_m; j++) {
                T sum = T();
                for (size_t k = 0; k < size_m; k++) {
                    sum += Get(i, k) * other.Get(k, j);
                }
                result.Set(i, j, sum);
            }
        }

        return result;
    }

    std::vector<T> MultiplyByVector(const std::vector<T>& vector) const {
        if (size_m != vector.size()) 
            throw std::invalid_argument("Matrix columns must match vector size");
        
        std::vector<T> result(size_n, T());

        for (size_t i = 0; i < size_n; i++) {
            T sum = T();

            for (size_t j = 0; j < size_m; j++) {
                sum += Get(i, j) * vector[j];
            }

            result[i] = sum;
        }

        return result;
    }

    std::vector<T> TransposeMultiplyByVector(const std::vector<T>& vector) const {
        if (size_n != vector.size())
            throw std::invalid_argument("Matrix rows must match vector size");
        
        std::vector<T> result(size_m, T());

        for (size_t j = 0; j < size_m; j++) {
            T sum = T();

            for (size_t i = 0; i < size_n; i++) {
                sum += Get(i, j) * vector[i];
            }

            result[j] = sum;
        }

        return result;
    }

    void SwapRows(size_t row1, size_t row2) {
        if (row1 == row2)
            return;
        if (row1 >= size_n || row2 >= size_n)
            throw std::out_of_range("Row index out of range");
        
        for (size_t j = 0; j < size_m; j++) {
            T temp = Get(row1, j);
            Set(row1, j, Get(row2, j));
            Set(row2, j, temp);
        }
    }

    void SwapColumns(size_t column1, size_t column2) {
        if (column1 == column2)
            return;
        if (column1 >= size_m || column2 >= size_m)
            throw std::out_of_range("Column index out of range");
        
        for (size_t i = 0; i < size_n; i++) {
            T temp = Get(i, column1);
            Set(i, column1, Get(i, column2));
            Set(i, column2, temp);
        }
    }

    double Norm() const {
        double sum = 0.0;

        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < size_m; j++) {
                double value = static_cast<double>(Get(i, j));
                sum += value * value;
            }
        }

        return std::sqrt(sum);
    }

    Matrix<T> Transpose() const {
        Matrix<T> result(size_m, size_n);

        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < size_m; j++) {
                result.Set(j, i, Get(i, j));
            }
        }

        return result;
    }
    
    void MultiplyRow(size_t row, const T& scalar) {
        if (row >= size_n)
            throw std::out_of_range("Index out of range");
        
        for (size_t j = 0; j < size_m; j++) {
            T value = Get(row, j) * scalar;
            Set(row, j, value);
        }
    }

    void MultiplyColumn(size_t column, const T& scalar) {
        if (column >= size_m)
            throw std::out_of_range("Index out of range");
        
        for (size_t i = 0; i < size_n; i++) {
            T value = Get(i, column) * scalar;
            Set(i, column, value);
        }
    }

    void AddRowMultiple(size_t target, size_t source, const T& scalar) {
        if (target >= size_n || source >= size_n)
            throw std::out_of_range("Row index out of range");

        for (size_t j = 0; j < size_m; j++) {
            T value = Get(target, j) + Get(source, j) * scalar;
            Set(target, j, value);
        }
    }

    void AddColumnMultiple(size_t target, size_t source, const T& scalar) {
        if (target >= size_m || source >= size_m)
            throw std::out_of_range("Column index out of range");
        
        for (size_t i = 0; i < size_n; i++) {
            T value = Get(i, target) + Get(i, source) * scalar;
            Set(i, target, value);
        }
    }

    template<typename Func>
    Matrix<T> Map(Func func) const {
        Matrix<T> result(size_n, size_m, T());

        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < size_m; j++) {
                result.Set(i, j, func(Get(i, j)));
            }
        }

        return result;
    }

    template<typename Func>
    T Reduce(Func func, T initial) const {
        T result = initial;

        for (size_t i = 0; i < size_n; i++) {
            for (size_t j = 0; j < size_m; j++) {
                result = func(result, Get(i, j));
            }
        }
        return result;
    }
};

#endif /* MATRIX_HPP */
