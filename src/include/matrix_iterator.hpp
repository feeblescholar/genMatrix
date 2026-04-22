#ifndef MATRIX_ITERATOR
#define MATRIX_ITERATOR

#include "matrix.hpp"

namespace genMatrix {
    /**
     * @class Matrix_Iterator
     * @brief Iterátor osztály a Matrix<T>-hez.
     * @details Ez az iterátor implementálja a C++ szabvány szerinti operátorokat, amiket a std::random_access_iterator
     *          megkövetel. A függvények nincsenek kommentelve, mert igazából pointer wrapperek.
     * @note Operátor hadsereg osztály.
     */
    template<typename T> class Matrix<T>::Matrix_Iterator {
        T* ptr;
        
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        /**
         * @note A konstruktor azért explicit, hogy megakadályozzuk az implicit típuskonverzióból származó bugokat. Például
         *       raw pointer adunk egy olyan helyre ahol iteratort várunk.
         */
        explicit Matrix_Iterator(T* _ptr = nullptr);

        reference operator*() const;
        pointer operator->() const;
        reference operator[](difference_type rhs_diff) const;

        Matrix_Iterator& operator++();
        Matrix_Iterator operator++(int);
        Matrix_Iterator& operator--();
        Matrix_Iterator operator--(int);

        Matrix_Iterator  operator+(difference_type rhs_diff) const;
        Matrix_Iterator  operator-(difference_type rhs_diff) const;
        Matrix_Iterator& operator+=(difference_type rhs_diff);
        Matrix_Iterator& operator-=(difference_type rhs_diff);

        difference_type operator-(const Matrix_Iterator& rhs_iter) const;

        /* Sajnos a C++ szabvány megköveteli ezt az operátort (ehhez az iterátorhoz), ezt meg csak friendként (vagy global) lehet implementálni. */
        friend Matrix_Iterator operator+(difference_type lhs_diff, const Matrix_Iterator& rhs_iter);
        
        bool operator==(const Matrix_Iterator& rhs_iter) const;
        bool operator!=(const Matrix_Iterator& rhs_iter) const;
        bool operator<=(const Matrix_Iterator& rhs_iter) const;
        bool operator>=(const Matrix_Iterator& rhs_iter) const;
        bool operator>(const Matrix_Iterator& rhs_iter) const;
        bool operator<(const Matrix_Iterator& rhs_iter) const;

    };
}

#endif