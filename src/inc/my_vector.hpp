#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>


template <typename T>
class my_vector
{
public:
    my_vector();                                        // default constructor
    my_vector(size_t sz);                               // constructor wih size parameter
    my_vector(const std::initializer_list<T>& list);    // constructor with initialization list
    my_vector(const my_vector<T>& obj);                 // copy constructor
    my_vector(my_vector<T>&& obj);                      // move constructor

    ~my_vector();

    T& operator[](size_t i);
    const T& operator[](size_t i) const;
    my_vector<T> operator=(const my_vector<T>& obj);
    my_vector<T> operator=(my_vector<T>&& obj);


    T& front();
    T& back();
    T& at(size_t index);
    T& begin();
    T& end();

    const T& front() const;
    const T& back() const;
    const T& at(size_t index) const;
    const T& begin() const;
    const T& end() const;

    size_t size() const noexcept;
    size_t capacity() const;
    void clear();
    bool empty() const;
    void pop_back();
    void push_back(const T& val);
    void push_back(T&& val);
    void insert(size_t pos, T& elem);
    void insert(size_t pos, T* begin, T* end);
    void emplace(size_t pos, T&& elem);
    void emplace_back(T&& val);
    void reserve(size_t capacity);
    void shrink_to_fit();
    void resize (size_t n);
private:
    size_t size_;
    size_t reserved_;
    T*  elem_;
    //typename std::aligned_storage<sizeof(T), alignof(T)>::type* data;
};




template <typename T>
my_vector<T>::my_vector()
    : size_{0}
    , reserved_{0}
    , elem_{nullptr}
{

}

template <typename T>
my_vector<T>::my_vector(const std::initializer_list<T>& list)
    : size_{list.size()}
    , reserved_{size_*2}
    , elem_{malloc(sizeof (T)*reserved_)}
    //, data{new typename std::aligned_storage<sizeof(T), alignof(T)>::type[reserved_]}
{
    std::copy(list.begin(), list.end(), elem_);
}

template <typename T>
my_vector<T>::my_vector(size_t sz)
    : size_{sz}
    , reserved_{size_*2}
    , elem_{malloc(sizeof (T)*reserved_)}
{
    if(sz > 0)
    {
        new (elem_) T[size_];
    }
}

template <typename T>
my_vector<T>::my_vector(const my_vector<T>& obj)
{
    size_ = obj.size();
    reserved_ = obj.capacity();
    elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
    std::copy(obj.begin(), obj.end(), elem_);
}

template <typename T>
my_vector<T>::my_vector(my_vector<T>&& obj)
{
    size_t tmp_size = size_;
    size_t tmp_reserv = reserved_;
    T* tmp_elem = elem_;
    try
    {
        size_ = obj.size();
        reserved_ = obj.capacity();
        elem_ = &obj[0];
        &obj[0] = nullptr;
        delete [] tmp_elem;
    }
    catch(...)
    {
        size_ = tmp_size;
        reserved_ = tmp_reserv;
        elem_ = tmp_elem;
    }
}

template <typename T>
my_vector<T>::~my_vector()
{
    delete[] elem_;
    elem_ = nullptr;
}

template <typename T>
T& my_vector<T>::operator[](size_t i)
{
    return elem_[i];
}

template <typename T>
const T& my_vector<T>::operator[](size_t i) const
{
    return elem_[i];
}

template <typename T>
my_vector<T> my_vector<T>::operator=(const my_vector<T>& obj)
{
    size_ = obj.size();
    reserved_ = obj.capacity();
    elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
    std::copy(obj.begin(), obj.end(), elem_);
    return *this;
}

template <typename T>
my_vector<T> my_vector<T>::operator=(my_vector<T>&& obj)
{
    size_t tmp_size = size_;
    size_t tmp_reserv = reserved_;
    T* tmp_elem = elem_;
    try
    {
        size_ = obj.size();
        reserved_ = obj.capacity();
        elem_ = &obj[0];
        &obj[0] = nullptr;
        delete [] tmp_elem;
    }
    catch(...)
    {
        size_ = tmp_size;
        reserved_ = tmp_reserv;
        elem_ = tmp_elem;
    }
    return *this;
}

template <typename T>
T& my_vector<T>::front()
{
    return elem_[0u];
}

template <typename T>
T& my_vector<T>::back()
{
    return elem_[size_-1];
}

template <typename T>
T& my_vector<T>::at(size_t index)
{
    if(index >= size_)
    {
        throw std::out_of_range("my_vector::at()");
    }
    return elem_[index];
}

template <typename T>
T& my_vector<T>::begin()
{
    return elem_[0]; // to do; replace with iterator
}

template <typename T>
T& my_vector<T>::end()
{
    return elem_[size_]; // to do; replace with iterator
}


template <typename T>
const T& my_vector<T>::front() const
{
    return elem_[0u];
}

template <typename T>
const T& my_vector<T>::back() const
{
    return elem_[size_-1];
}

template <typename T>
const T& my_vector<T>::at(size_t index) const
{
    if(index >= size_)
    {
        throw std::out_of_range("my_vector::at()");
    }
    return elem_[index];
}

template <typename T>
const T& my_vector<T>::begin() const // to do; replace with iterator
{
    return elem_[0];
}

template <typename T>
const T& my_vector<T>::end() const // to do; replace with iterator
{
    return elem_[size_];
}

template <typename T>
void my_vector<T>::clear()
{
    for (T* iter = elem_; iter != elem_+size_; ++iter)
    {
        iter->~T();
    }
    size_ = 0u;
}

template <typename T>
size_t my_vector<T>::size() const noexcept
{
    return size_;
}

template <typename T>
size_t my_vector<T>::capacity() const
{
    return reserved_;
}
template <typename T>
bool my_vector<T>::empty() const
{
    return (size_ > 0) ? false : true;
}

template <typename T>
void my_vector<T>::pop_back()
{
    if(size_ > 0)
    {
        elem_[--size_].~T();
    }
}

template <typename T>
void my_vector<T>::push_back(const T& val)
{
    if(size_ < reserved_)
    {
        new (elem_+size_) T(val);
        ++size_;
    }
    else
    {
        T* tmp = elem_;
        reserved_ = reserved_*2;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
        std::copy(tmp, tmp + size_, elem_);
        new (elem_+size_) T(val);
        delete [] tmp;
    }
}

template <typename T>
void my_vector<T>::push_back(T&& val)
{
    if(size_ < reserved_)
    {
        new (elem_+size_) T(std::forward<T>(val)); // ??? из lvalue делаю rvalue для инициализации конструктором перемещения
        ++size_;
    }
    else
    {
        T* tmp = elem_;
        reserved_ = reserved_*2;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
        std::copy(tmp, tmp + size_, elem_);
        new (elem_+size_) T(std::forward<T>(val));
        delete [] tmp;
    }
}

template <typename T>
void my_vector<T>::insert(size_t pos, T& elem)
{
    if(pos < size_)
    {
        if(size_ >= reserved_)
        {
           reserved_ = size_*2;
        }

        T* tmp = elem_;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use

        std::copy(tmp, tmp + pos, elem_);
        new (elem_+pos) T(elem);
        std::copy(tmp + pos, tmp + size_, elem_ + pos + 1);
        ++size_;
    }
    else
    {
        throw std::out_of_range("insertion pos is out of range");
    }
}

template <typename T>
void my_vector<T>::insert(size_t pos, T* begin, T* end)
{
    if(pos < size_)
    {
        size_t insert_size = end - begin;
        size_t new_size = size_ + insert_size;

        if(new_size >= reserved_)
        {
           reserved_ = 2 * (size_ + insert_size) ;
        }

        T* tmp = elem_;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
        std::copy(tmp, tmp + pos, elem_);
        std::copy(begin, end, elem_ + pos);
        std::copy(tmp + pos, tmp + size_, elem_ + pos + insert_size);
        size_ = new_size;
    }
    else
    {
        throw std::out_of_range("insertion pos is out of range");
    }
}

template <typename T>
void my_vector<T>::emplace(size_t pos, T&& elem)
{
    if(pos < size_)
    {
        if(size_ >= reserved_)
        {
           reserved_ = size_*2;
        }

        T* tmp = elem_;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use

        std::copy(tmp, tmp + pos, elem_);
        new (elem_+pos) T(std::forward<T>(elem)); // ??? из lvalue делаю rvalue для инициализации конструктором перемещения
        std::copy(tmp + pos, tmp + size_, elem_ + pos + 1);
        ++size_;
    }
    else
    {
        throw std::out_of_range("insertion pos is out of range");
    }
}

template <typename T>
void my_vector<T>::emplace_back(T&& val)
{
    if(size_ < reserved_)
    {
        new (elem_+size_) T(std::forward<T>(val)); // ??? из lvalue делаю rvalue для инициализации конструктором перемещения
        ++size_;
    }
    else
    {
        T* tmp = elem_;
        reserved_ = reserved_*2;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
        std::copy(tmp, tmp + size_, elem_);
        new (elem_+size_) T(std::forward<T>(val));
        delete [] tmp;
    }
}

template <typename T>
void my_vector<T>::reserve(size_t capacity)
{
    if(capacity > reserved_)
    {
        reserved_ = capacity;
        T* tmp = elem_;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
        std::copy(tmp, tmp + size_, elem_);
        delete [] tmp;
    }
}

template <typename T>
void my_vector<T>::shrink_to_fit()
{
    if(size_ < reserved_)
    {
        reserved_ = size_;
        T* tmp = elem_;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
        std::copy(tmp, tmp + size_, elem_);
        delete [] tmp;
    }
}

template <typename T>
void my_vector<T>::resize(size_t n)
{
    if(n < size_)
    {
        for(T* iter = elem_ + n; iter != elem_+size_; ++iter )
        {
            iter->~T();
        }
    }
    else if(n > reserved_)
    {
        T* tmp = elem_;
        reserved_ = 2 * n;
        elem_ = malloc(sizeof (T)*reserved_); //std::aligned_storage need to use
        std::copy(tmp, tmp + size_, elem_);
        new (elem_ + size_) T[n-size_];
        delete [] tmp;
    }
    else
    {
        new (elem_ + size_) T[n-size_];
    }
    size_ = n;
}
#endif // MY_VECTOR_HPP
