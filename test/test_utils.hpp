#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <random>

template<typename T, bool negzero = true> T random() {
    int lbound = -50;
    if (!negzero) lbound = 10;
    const int ubound = 50;

    std::default_random_engine eng(std::random_device{}());

    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> d(lbound, ubound);
        return d(eng);
    }
    else {
        std::uniform_real_distribution<T> d(lbound, ubound);
        return d(eng);
    }
}

template<typename T> T relaxed_epsilon() {
    if constexpr (std::is_integral_v<T>) return T(0);
    else if (std::is_same_v<T, double>) return 1e-10;
    else return 1e-4;
} 

/**
 * Egy fix tömb, random számokkal.
 * Kizárólag teszteléshez van, sok mindent nem ellenőriz.
 */
template<typename T, size_t n = 1, bool neg = true> struct TestArray {
    T arr[n];
    size_t s;

    TestArray() : s(n) {
        for (size_t i = 0; i < n; i++) arr[i] = random<T, neg>();
    }

    T& operator[](const size_t idx) {
        if (idx >= s) throw "tulindexeles";
        return arr[idx]; 
    }

    const T& operator[](const size_t idx) const {
        if (idx >= s) throw "tulindexeles";
        return arr[idx]; 
    }

    void fillmat(genMatrix::Matrix<T>& mtx){
        if (s > mtx.size()) throw "nem fer bele";
        for (size_t k = 0; k < mtx.size(); k++) {
            size_t i = k / mtx.getCols();
            size_t j = k % mtx.getCols();
            mtx(i, j) = arr[k];
        }
    }
};

#endif