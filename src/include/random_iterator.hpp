/**
 * @file random_iterator.hpp
 * @author Kovács Botond
 * @brief Generikus teljesen implementált random access iterátor.
 */
#ifndef RANDOM_ITERATOR_H
#define RANDOM_ITERATOR_H

#include <iterator>

namespace genMatrix::internal::types {
/**
 * @class RandomIterator
 * @brief Osztálysablon a genMatrix tárolóinak iterátoraihoz.
 * @details Ez az iterátor implementálja a C++ szabvány szerinti operátorokat, 
 *          amiket a std::random_access_iterator megkövetel. A függvények nincsenek 
 *          kommentelve, mert igazából pointer wrapperek.
 * @tparam T Maga a típus
 * @tparam P A típus mutatója
 * @tparam R A típus referenciája
 * @note Operátor hadsereg osztály.
 */
template<typename T, typename P, typename R>
class RandomIterator {
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
    explicit RandomIterator(P _ptr = nullptr) : ptr(_ptr) {}

    /** Ez itt a constot megoldja, lecastoljuk ha kell. */
    template<typename CT, typename OP, typename OR>
    RandomIterator(const RandomIterator<CT, OP, OR>& other) : ptr(other.operator->()) {}

    reference operator*() const noexcept { 
        return *ptr; 
    }

    pointer operator->() const noexcept  { 
        return ptr;  
    }

    reference operator[](difference_type rhs_diff) const noexcept {
        return ptr[rhs_diff]; 
    }

    RandomIterator& operator++() noexcept { 
        ptr++;
        return *this;
    }

    RandomIterator operator++(int) noexcept {
        RandomIterator tmp = *this;
        ++(tmp);
        return tmp;
    }

    RandomIterator& operator--() noexcept {
        ptr--;
        return *this;
    }

    RandomIterator operator--(int) noexcept {
        RandomIterator tmp = *this;
        --(tmp);
        return tmp;
    }

    RandomIterator operator+(difference_type rhs_diff) const noexcept {
        RandomIterator rval = *this;
        rval += rhs_diff;
        return rval;
    }

    RandomIterator operator-(difference_type rhs_diff) const noexcept {
        RandomIterator rval = *this;
        rval -= rhs_diff;
        return rval;
    }

    RandomIterator& operator+=(difference_type rhs_diff) noexcept {
        ptr += rhs_diff;
        return *this;
    }

    RandomIterator& operator-=(difference_type rhs_diff) noexcept {
        ptr -= rhs_diff;
        return *this;
    }

    difference_type operator-(const RandomIterator& rhs_iter) const noexcept {
        return ptr - rhs_iter.ptr;
    }

    /** 
     * @note A C++ szabvány megköveteli ezt az operátort, ezt meg csak 
     *       friend/global lehet implementálni. 
     */
    friend RandomIterator operator+(difference_type lhs_diff, 
                                    const RandomIterator& rhs_iter) noexcept 
    {
        return rhs_iter + lhs_diff;
    }
    
    template<typename P2, typename R2>
    bool operator==(const RandomIterator<T, P2, R2>& rhs_iter) const noexcept {
        return ptr == rhs_iter.operator->(); 
    }

    template<typename P2, typename R2>
    bool operator!=(const RandomIterator<T, P2, R2>& rhs_iter) const noexcept {
        return ptr != rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator<=(const RandomIterator<T, P2, R2>& rhs_iter) const noexcept {
        return ptr <= rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator>=(const RandomIterator<T, P2, R2>& rhs_iter) const noexcept {
        return ptr >= rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator>(const RandomIterator<T, P2, R2>& rhs_iter) const noexcept {
        return ptr > rhs_iter.operator->();
    }

    template<typename P2, typename R2>
    bool operator<(const RandomIterator<T, P2, R2>& rhs_iter) const noexcept {
        return ptr < rhs_iter.operator->();
    }
};
}

#endif