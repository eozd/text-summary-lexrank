#pragma once

#include <cassert>
#include <vector>

namespace ir {

/**
 * @brief A vector (sequence) of T values.
 *
 * @tparam T Type of the values in the sequence.
 */
template <typename T> class Vector {
  public:
    /**
     * @brief Type of the values stored in this vector type.
     */
    using value_type = T;

    /**
     * @brief Default constructor constructing an empty vector.
     */
    Vector() = default;

    /**
     * @brief Construct a vector of size many default initialized elements.
     *
     * @param size Number of elements.
     */
    explicit Vector(size_t size) : m_size(size), m_data(size, T()) {}

    /**
     * @brief Return a reference to ith element in the given vector (0-based
     * indexing)
     *
     * @param i Index of the element.
     * @return Reference to ith element.
     */
    T& operator()(size_t i) { return m_data[i]; };

    /**
     * @brief Return a const-reference to ith element in the given vector
     * (0-based indexing)
     *
     * @param i Index of the element
     * @return const-reference to ith element
     */
    const T& operator()(size_t i) const { return m_data[i]; }

    /**
     * @brief Return pointer to the beginning of the data sequence stored by
     * this vector object.
     *
     * @brief Pointer to the beginning of the data segment.
     */
    T* data() { return m_data.data(); }

    /**
     * @brief Return const pointer to the beginning of the data sequence stored
     * by this vector object.
     *
     * @return const pointer to the beginning of the data segment
     */
    const T* data() const { return m_data.data(); }

    /**
     * @brief Return the number of elements of this vector.
     *
     * @return Number of elements
     */
    size_t size() const { return m_size; }

  private:
    /**
     * @brief Number of elements
     */
    size_t m_size;

    /**
     * @brief Underlying data container
     */
    std::vector<T> m_data;
};

/**
 * @brief A <b>row-major</b> matrix of T values.
 *
 * @tparam T Type of the values stored.
 */
template <typename T> class Matrix {
  public:
    /**
     * @brief Type of the values stored by this Matrix type.
     */
    using value_type = T;

    /**
     * @brief Default constructor initializing an empty matrix object.
     */
    Matrix() = default;

    /**
     * @brief Construct a matrix of shape (rows)x(cols) with default initialized
     * elements.
     *
     * @param rows Number of rows of the matrix.
     * @param cols Number of columns of the matrix.
     */
    Matrix(size_t rows, size_t cols)
        : n_rows(rows), n_cols(cols), m_data(n_rows * n_cols, T()) {}

    /**
     * @brief Return a reference to \f$X_{ij}\f$ where \f$X\f$ is this
     * matrix object.
     *
     * @param i Row index of the element.
     * @param j Column index of the element.
     *
     * @return Reference to ijth element.
     */
    T& operator()(size_t i, size_t j) { return m_data[i * n_cols + j]; }

    /**
     * @brief Return a const-reference to \f$X_{ij}\f$ where \f$X\f$ is this
     * matrix object.
     *
     * @param i Row index of the element.
     * @param j Column index of the element.
     *
     * @return const-reference to ijth element.
     */
    const T& operator()(size_t i, size_t j) const {
        return m_data[i * n_cols + j];
    }

    /**
     * @brief Return a pointer to the beginning of the <b>row-major</b> data
     * sequence stored by this matrix object.
     *
     * @return const pointer to the beginning of the data segment
     */
    T* data() { return m_data.data(); }

    /**
     * @brief Return const pointer to the beginning of the <b>row-major</b> data
     * sequence stored by this matrix object.
     *
     * @return const pointer to the beginning of the data segment
     */
    const T* data() const { return m_data.data(); }

    /**
     * @brief Get number of rows of this matrix.
     *
     * @return Number of rows.
     */
    size_t rows() const { return n_rows; }

    /**
     * @brief Get number of columns of this matrix.
     *
     * @return Number of columns.
     */
    size_t cols() const { return n_cols; }

  private:
    /**
     * @brief Number of rows.
     */
    size_t n_rows;

    /**
     * @brief Number of columns.
     */
    size_t n_cols;

    /**
     * @brief Underlying data container.
     */
    std::vector<T> m_data;
};

/**
 * @brief Perform a matrix-vector multiplication and return the result as a new
 * vector.
 *
 * @tparam T Type of the matrix and vector.
 * @param matrix Matrix to multiply.
 * @param vector Vector to multiply.
 * @return Result of the matrix-vector multiplication.
 */
template <typename T>
Vector<T> operator*(const Matrix<T>& matrix, const Vector<T>& vector) {
    assert(matrix.cols() == vector.size());

    Vector<T> result(matrix.rows());
    for (size_t i = 0; i < matrix.rows(); ++i) {
        T elem = T();
        for (size_t j = 0; j < matrix.cols(); ++j) {
            elem += matrix(i, j) * vector(j);
        }
        result(i) = elem;
    }

    return result;
}

} // namespace ir
