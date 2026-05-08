#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <random>
#include <gtest/gtest.h>

/**
 * @brief Típusfüggő RNG (Random Number Generator).
 * @tparam negzero Negatív számok és nulla engedélyezése.
 */
template<typename T, bool negzero = true>
T random() {
    int lbound = -10;
    if (!negzero) lbound = 1;
    const int ubound = 10;

    static std::default_random_engine eng(std::random_device{}());

    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> d(lbound, ubound);
        return d(eng);
    }
    else if constexpr (std::is_same_v<T, genMatrix::Complex>) {
        std::uniform_real_distribution<double> d(lbound, ubound);
        return genMatrix::Complex(d(eng), d(eng));
    }
    else {
        std::uniform_real_distribution<T> d(lbound, ubound);
        return d(eng);
    }
}

template<typename T, bool negzero = true>
T rng = random<T, negzero>();

/**
 * @brief Típusfüggő lazább epsilon értéket ad vissza.
 * @details Egyes számítások nem tudnak olyan pontosak lenni FPU aritmetikai
 *          korlátok miatt. Ezért ez a fv. ad egy lazább epsilont vissza.
 */
template<typename T>
constexpr T relaxed_epsilon() {
    if constexpr (std::is_integral_v<T>) 
        return 0;
    else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, genMatrix::Complex>) 
        return 1e-10;
    else 
        return 1e-3;
} 

/**
 * @brief Típusfüggő lazább epsilon értéket ad vissza.
 * @details Egyes számítások nem tudnak olyan pontosak lenni FPU aritmetikai
 *          korlátok miatt. Ezért ez a fv. ad egy lazább epsilont vissza.
 */
template<typename T>
constexpr T eps_l = relaxed_epsilon<T>();

/**
 * @class TestArray<T>
 * @brief Egy fix méretű (de dinamikusan foglalt) tömb, random számokkal.
 * @tparam T A tömb típusa
 * @tparam n A tömb mérete
 * @tparam neg Nem pozitív számok generálásának engedése.
 * @note Azért nem egy random mátrix, mert így a generált értékekhez, utólag is 
 *       hozzá lehet férni.
 * @warning Kizárólag teszteléshez van, sok mindent nem ellenőriz.
 */
template<typename T, size_t n = 1, bool neg = true> 
struct TestArray {
    T* arr;
    size_t s;

    TestArray() : s(n) {
        arr = new T[n];
        for (size_t i = 0; i < n; i++) 
            arr[i] = random<T, neg>();
    }

    T& operator[](const size_t idx) {
        if (idx >= s) throw "tulindexeles";
        return arr[idx]; 
    }

    const T& operator[](const size_t idx) const {
        if (idx >= s) throw "tulindexeles";
        return arr[idx]; 
    }

    /**
     * @brief Feltölt egy mátrixot a tömb (arr) tartalmával.
     * @param mtx A feltöltendő mátrix referenciája
     */
    void mtx_fill(genMatrix::Matrix<T>& mtx) const {
        if (s > mtx.size()) throw "nem fer bele";
        for (size_t k = 0; k < mtx.size(); k++) {
            size_t i = k / mtx.getCols();
            size_t j = k % mtx.getCols();
            mtx(i, j) = arr[k];
        }
    }

    /**
     * @brief Feltölt egy vektort a tömb (arr) tartalmával.
     * @param vec A feltöltendő vektor referenciája.
     */
    void vec_fill(genMatrix::Vector<T>& vec) const {
        for (size_t k = 0; k < s; k++) {
            vec.push_back(arr[k]);
        }
    }

    ~TestArray() {
        delete[] arr;
    }
};

/**
 * Tesztelésnél használt komparátor.
 * Minden értéket ellenőriz az EXPECT_NEAR makróval.
 */
template<typename T>
bool mtx_cmp_test(const genMatrix::Matrix<T>& a, const genMatrix::Matrix<T>& b, 
        const T epsilon = std::numeric_limits<T>::epsilon() * 1e3) {
    if (&a == &b) 
        return true; 

    if ((a.getRows() != b.getRows()) || (a.getCols() != b.getCols())) 
        return false;
        
    if (a.getRows() == 0 && a.getCols() == 0) 
        return true;
    
    for (size_t i = 0; i < a.getRows(); i++)
        for (size_t j = 0; j < a.getCols(); j++)
            EXPECT_NEAR(a(i, j), b(i, j), epsilon);

    return true;
}

/**
 * Tesztelésnél használt komparátor.
 * Minden értéket ellenőriz az EXPECT_NEAR makróval.
 */
template<typename T>
bool vec_cmp_test(const genMatrix::Vector<T>& a, const genMatrix::Vector<T>& b, 
        const T epsilon = std::numeric_limits<T>::epsilon() * 1e3) {
    if (&a == &b) 
        return true; 

    if (a.size() != b.size()) 
        return false;

    for (size_t i = 0; i < a.size(); i++)
            EXPECT_NEAR(a[i], b[i], epsilon); 

    return true;
}

#endif