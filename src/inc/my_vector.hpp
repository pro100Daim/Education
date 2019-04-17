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
private:
    size_t size_;
    size_t reserved_;
    T*  elem_;
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
    , reserved_{1.5 * size_}
    , elem_{new T[reserved_]}
{
    std::copy(list.begin(), list.end(), elem_);
}

template <typename T>
my_vector<T>::my_vector(size_t sz)
    : size_{sz}
    , reserved_{1.5 * size_}
{
    if(sz > 0)
    {
        elem_ = new T[reserved_];
        for (auto &var : elem_)
        {
            var = T();
        }
    }
}

template <typename T>
my_vector<T>::my_vector(const my_vector<T>& obj)
{
    size_ = obj.size();
    reserved_ = obj.capacity();
    elem_ = new T(reserved_);
    for(int i = 0; i < reserved_; ++i)
    {
        elem_[i] = obj[i];
    }
    //std::copy(obj.begin(), obj.end(), elem_);
}

template <typename T>
my_vector<T>::my_vector(my_vector<T>&& obj)
{
    size_t tmp_size = size_;
    size_t tmp_reserv = reserved_;
    T tmp_elem[] = elem_;
    try
    {
        size_ = obj.size();
        reserved_ = obj.capacity();
        elem_ = &obj[0];
        &obj[0] = nullptr;
        obj.clear();
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
    elem_ = new T(reserved_);
    for(int i = 0; i < reserved_; ++i)
    {
        elem_[i] = obj[i];
    }
    for(int i = 0; i < reserved_; ++i)
    {
        elem_[i] = obj[i];
    }
    //std::copy(obj.begin(), obj.end(), elem_);
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
        obj.clear();
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
void my_vector<T>::clear()
{
    size_ = 0u;
    reserved_ = 0u;
    delete [] elem_;
    elem_ = nullptr;
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


#endif // MY_VECTOR_HPP
