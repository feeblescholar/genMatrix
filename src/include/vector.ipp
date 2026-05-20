#ifndef VECTOR_I
#define VECTOR_I

namespace genMatrix {

template<typename T>
Vector<T>::Vector(size_t n) : data(nullptr), _size(0), _capacity(n) {
    if (n == 0)
        throw Vector_Error("[Constructor]", "Initial capacity must be >=1");
    data = new T[n];
}

template<typename T>
Vector<T>::Vector(const Matrix<T>& mtx, const size_t idx) 
    : data(nullptr)
    , _size(mtx.getRows()) 
    , _capacity(mtx.getRows()) 
{
    data = new T[_size];
    for (size_t i = 0; i < _size; i++)
        data[i] = mtx(i, idx);
}

template<typename T>
Vector<T>::Vector(const Vector<T>& other) 
    : data(nullptr) 
    , _size(other._size)
    , _capacity(other._capacity) 
{
    *this = other;
}

template<typename T>
template<typename S>
Vector<T>::Vector(const Vector<S>& other) 
    : data(nullptr) 
    , _size(other._size)
    , _capacity(other._capacity) 
{
    *this = other;
}

template<typename T>
Vector<T>::Vector(Vector<T>&& other) : data(nullptr), _size(0), _capacity(0) {
    data = std::exchange(other.data, nullptr);
    _size = std::exchange(other._size, 0);
    _capacity = std::exchange(other._capacity, 0);
}

template<typename T>
Vector<T>::~Vector() {
    if (data) 
        delete[] data;
    
    data = nullptr;
    _size = _capacity = 0;
}

template<typename T>
template<typename S>
Vector<T>::operator Matrix<S>() const {
    Matrix<S> ret(_size, 1);

    for (size_t i = 0; i < _size; i++)
        ret(i, 0) = static_cast<S>(data[i]);

    return ret;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other)
        *this = this->operator=<T>(other);

    return *this;
}

template<typename T>
template<typename S>
Vector<T>& Vector<T>::operator=(const Vector<S>& other) {
    if (data) 
        delete[] data;

    data = new T[other.capacity()];
    _capacity = other.capacity();
    _size = other.size();

    for (size_t i = 0; i < _size; i++) {
        data[i] = static_cast<T>(other[i]);
    }

    return *this;
}

template<typename T>
const T& Vector<T>::operator[](const size_t idx) const {
    if (idx >= _size)
        throw Vector_Error("operator[]", "Out of index.");
    
    return data[idx];
}

template<typename T>
T& Vector<T>::operator[](const size_t idx) {
    if (idx >= _size)
        throw Vector_Error("[operator[]]", "Out of index.");
    
    return data[idx];
}

template<typename T>
Vector<T>& Vector<T>::push_back(const T& val) {
    if (_size + 1 > _capacity) {
        T* ndata = new T[_capacity * 2];
        for (size_t i = 0; i < _size; i++)
            ndata[i] = data[i];

        _capacity *= 2;
        delete[] data;
        data = ndata;
    }

    data[_size++] = val;
    return *this;
}

template<typename T>
void Vector<T>::resize(const size_t ncap) {
    if (ncap < _size)
        throw "hibaosztaly ide";
    
    if (ncap == _capacity)
        return;

    T* ndata = new T[ncap];
    if (_size != 0) 
        for (size_t i = 0; i < _size; i++)
            ndata[i] = data[i];
    
    _capacity = ncap;
    delete[] data;
    data = ndata;
}

template<typename T>
void Vector<T>::shrink() {
    if (_size == 0) {
        delete[] data;
        data = nullptr;
        _capacity = 0;
        return;
    }

    T* ndata = new T[_size];
    for (size_t i = 0; i < _size; i++)
        ndata[i] = data[i];
    
    _capacity = _size;
    delete[] data;
    data = ndata;
}

template<typename T>
template<typename S>
bool Vector<T>::operator==(const Vector<S>& rhs_vec) const {
    if (!std::is_same_v<T, S>)
        return false;

    if (_size != rhs_vec._size)
        return false;
    
    for (size_t i = 0; i < _size; i++) 
        if (data[i] != rhs_vec[i])
            return false;
    
    return true;
}

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator+(const Vector<S>& rhs_vec) const {
    if constexpr (has_add_v<T, S>) {
        using ReturnType = decltype(T(0) + S(0));

        Vector<ReturnType> rval = *this;
        Vector<ReturnType> tmp = rhs_vec;

        rval += tmp;
        return rval;
    }
    else
        throw Vector_Error("[operator+]", "Addition is undefined.");
}

template<typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& rhs_vec) {
    if constexpr (has_add_v<T, T>) {
        if (_size != rhs_vec._size)
            throw Vector_Error("[operator+=/-=]", "Must be the same size.");
        
        for (size_t i = 0; i < _size; i++)
            data[i] += rhs_vec[i];
    }
    else
        throw Vector_Error("[operator+=]", "Addition is undefined.");

    return *this;
}

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator-(const Vector<S>& rhs_vec) const {
    if constexpr (has_add_v<T, S> && has_mul_v<S, int>) {
        using ReturnType = decltype(T(0) + S(0));
        
        Vector<ReturnType> rval = *this;
        Vector<ReturnType> tmp = rhs_vec;

        rval += rhs_vec * -1;
        return rval;
    }
    else
        throw Vector_Error("[operator-]", "Required operators are undefined.");
}

template<typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& rhs_vec) {
    if constexpr (has_add_v<T, T> && has_mul_v<T, int>)
        *this += rhs_vec * -1;
    else
        throw Vector_Error("[operator-=]", "Required operators are undefined.");
    
    return *this;
}

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator*(const Vector<S>& rhs_vec) const {
    if constexpr (has_add_v<T, S> && has_mul_v<T, S>) {
        if (_size != rhs_vec.size())
            throw Vector_Error("[operator*]", "Must be the same size.");

        using ReturnType = decltype(T(0) * S(0));
        ReturnType rval = 0;

        for (size_t i = 0; i < _size; i++)
            rval += data[i] * rhs_vec[i];

        return rval;
    }
    else
        throw Vector_Error("[operator*]", "Required operators are undefined.");
}

template<typename T>
template<typename S>
decltype(auto) Vector<T>::operator*(const S& rhs_type) const {
    if constexpr (has_mul_v<T, S>) {
        using ReturnType = decltype(T(0) * S(0));

        Vector<ReturnType> rval = *this;
        ReturnType rhs_type_c = static_cast<ReturnType>(rhs_type);

        for (size_t i = 0; i < _size; i++) {
            rval[i] *= rhs_type_c;
        }

        return rval;
    }
    else
        throw Vector_Error("[operator*]", "Multiplication is undefined.");
}
}

#endif
