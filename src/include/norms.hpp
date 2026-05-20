/**
 * @file norms.hpp
 * @author Kovács Botond
 * @brief Mátrixok normáinak kiszámítása.
 */
#ifndef NORMS_H
#define NORMS_H

#include "matrix.hpp"
#include <numeric>

namespace genMatrix {

/**
 * @param vec - A normált vektor referenciája.
 * @return A vektor 1-normája.
 */
template<typename T>
auto norm_1(const Vector<T>& vec) {
    using ReturnType = decltype(utils::abs(T(0)));
    Vector<ReturnType> absVec = vec;

    std::transform(vec.begin(), vec.end(), absVec.begin(), utils::abs<T>);

    return std::accumulate(absVec.begin(), absVec.end(), 0);
}

/**
 * @param mtx - A normált mátrix referenciája.
 * @return A mátrix 1-normája.
 */
template<typename T>
auto norm_1(const Matrix<T>& mtx) {
    using ReturnType = decltype(utils::abs(T(0)));
    Vector<ReturnType> sumVector(mtx.getCols());

    for (size_t i = 0; i < mtx.getCols(); i++)
       sumVector.push_back(norm_1(Vector<T>(mtx, i)));

    return *std::max_element(sumVector.begin(), sumVector.end());
}

/**
 * @param vec - A normált vektor referenciája.
 * @return A vektor végtelen (maximum) normája.
 */
template<typename T>
auto norm_inf(const Vector<T>& vec) {
    using ReturnType = decltype(utils::abs(T(0)));
    Vector<ReturnType> absVec = vec;

    std::transform(vec.begin(), vec.end(), absVec.begin(), utils::abs<T>);

    return *std::max_element(absVec.begin(), absVec.end());
}

/**
 * @param mtx - A normált mátrix referenciája.
 * @return A mátrix végtelen (maximum) normája.
 */
template<typename T>
auto norm_inf(const Matrix<T>& mtx) {
    return norm_1(mtx.transpose());
}

/**
 * @param mtx - A normált mátrix referenciája.
 * @return A mátrix Frobenius normája.
 * @note Minimum floatra lépteti elő az eredményt.
 */
template<typename T>
auto norm_frobenius(const Matrix<T>& mtx) {
    using ReturnType = decltype(utils::abs(T(0)) + float(0));
    ReturnType norm = ReturnType(0);

    for (auto item : mtx)
        norm += utils::abs(item) * utils::abs(item);
    
    return utils::sqrt(norm);
}

/**
 * @param vec - A normált vektor referenciája.
 * @return A vektor Frobenius normája.
 * @note Minimum floatra lépteti elő az eredményt.
 */
template<typename T>
auto norm_frobenius(const Vector<T>& vec) {
    using ReturnType = decltype(utils::abs(T(0)) + float(0));

    ReturnType norm = ReturnType(0);

    for (auto item : vec)
        norm += utils::abs(item) * utils::abs(item);
    
    return utils::sqrt(norm);
}
}

#endif
