/**
 * @file matrix_iterator.hpp
 * @author Kovács Botond
 * @brief A generikus mátrixhoz tartozó iterátor osztály.
 */
#ifndef MATRIX_ITERATOR
#define MATRIX_ITERATOR

#include "matrix.hpp"

namespace genMatrix {
template<typename T> class Matrix; 

/**
 * @class Matrix_Iterator
 * @brief Iterátor osztály a Matrix<T>-hez.
 * @details Ez az iterátor implementálja a C++ szabvány szerinti operátorokat, 
 *          amiket a std::random_access_iterator megkövetel. A függvények nincsenek 
 *          kommentelve, mert igazából pointer wrapperek.
 * @note Operátor hadsereg osztály.
 */
template<typename T>
template<typename P, typename R>
class Matrix<T>::Matrix_Iterator {
    P ptr;
    
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = P;
    using reference = R;
    
    /**
     * @note A konstruktor azért explicit, hogy megakadályozzuk az implicit 
     *       típuskonverzióból származó bugokat. Például raw pointert adunk egy 
     *       olyan helyre ahol iteratort várunk.
     */
    explicit Matrix_Iterator(P _ptr = nullptr) : ptr(_ptr) {}

    /** Ez itt a constot megoldja, lecastoljuk ha kell. */
    template<typename OP, typename OR>
    Matrix_Iterator(const Matrix_Iterator<OP, OR>& other) : ptr(other.operator->()) {}

    reference operator*() const noexcept { 
        return *ptr; 
    }

    pointer operator->() const noexcept  { 
        return ptr;  
    }

    reference operator[](difference_type rhs_diff) const noexcept {
        return ptr[rhs_diff]; 
    }

    Matrix_Iterator& operator++() noexcept { 
        ptr++;
        return *this;
    }

    Matrix_Iterator operator++(int) noexcept {
        Matrix_Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    Matrix_Iterator& operator--() noexcept {
        ptr--;
        return *this;
    }

    Matrix_Iterator operator--(int) noexcept {
        Matrix_Iterator tmp = *this;
        --(*this);
        return tmp;
    }

    Matrix_Iterator operator+(difference_type rhs_diff) const noexcept {
        Matrix_Iterator rval = *this;
        rval += rhs_diff;
        return rval;
    }

    Matrix_Iterator operator-(difference_type rhs_diff) const noexcept {
        Matrix_Iterator rval = *this;
        rval -= rhs_diff;
        return rval;
    }

    Matrix_Iterator& operator+=(difference_type rhs_diff) noexcept {
        ptr += rhs_diff;
        return *this;
    }

    Matrix_Iterator& operator-=(difference_type rhs_diff) noexcept {
        ptr -= rhs_diff;
        return *this;
    }

    difference_type operator-(const Matrix_Iterator& rhs_iter) const noexcept {
        return ptr - rhs_iter.ptr;
    }

    /** A C++ szabvány megköveteli ezt az operátort, ezt meg csak friend/global lehet implementálni. */
    friend Matrix_Iterator operator+(difference_type lhs_diff, const Matrix_Iterator& rhs_iter) noexcept {
        return rhs_iter + lhs_diff;
    }
    
    template<typename P2, typename R2>
    bool operator==(const Matrix_Iterator<P2, R2>& rhs_iter) const noexcept {
        return ptr == rhs_iter.operator->(); 
    }

    template<typename P2, typename R2>
    bool operator!=(const Matrix_Iterator<P2, R2>& rhs_iter) const noexcept  {
        return ptr != rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator<=(const Matrix_Iterator<P2, R2>& rhs_iter) const noexcept  {
        return ptr <= rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator>=(const Matrix_Iterator<P2, R2>& rhs_iter) const noexcept  {
        return ptr >= rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator>(const Matrix_Iterator<P2, R2>& rhs_iter) const noexcept  {
        return ptr > rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator<(const Matrix_Iterator<P2, R2>& rhs_iter) const noexcept  {
        return ptr < rhs_iter.operator->();
    }
};
}

#endif
