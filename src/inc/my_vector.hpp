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

    T& operator[](int i);
    void operator=(const my_vector<T>& obj);
    void operator=(my_vector<T>&& obj);
    size_t size() const noexcept;
    T& begin();
    T& end();

private:
    size_t size_;
    T*  elem_;
};




template <typename T>
my_vector<T>::my_vector()
    : size_{0}
{

}

template <typename T>
my_vector<T>::~my_vector()
{
    delete[] elem_;
}

template <typename T>
my_vector<T>::my_vector(const std::initializer_list<T>& list)
    : size_{list.size()}
    , elem_{new T[list.size()]}
{
    std::copy(list.begin(), list.end(), elem_);
}

template <typename T>
my_vector<T>::my_vector(size_t sz)
    : size_{sz}
{
    if(sz < 0)
    {
        throw std::length_error("my_vector(sz)");
    }

    elem_ = new T[sz];
    for (auto &var : elem_) {
        var = 0;
    }
}

template <typename T>
my_vector<T>::my_vector(const my_vector<T>& obj)
{
    size_ = obj.size();
    elem_ = new T(size_);
    std::copy(obj.begin(), obj.end(), elem_);
}

template <typename T>
my_vector<T>::my_vector(my_vector<T>&& obj)
{
    //std::swap ???
}

template <typename T>
T& my_vector<T>::operator[](int i)
{
    if((i < 0) || (i > size()))
    {
        throw std::out_of_range("my_vector::operator[]");
    }
    return elem_[i];
}

template <typename T>
void my_vector<T>::operator=(const my_vector<T>& obj)
{

}
template <typename T>
void my_vector<T>::operator=(my_vector<T>&& obj)
{

}

template <typename T>
size_t my_vector<T>::size() const noexcept
{
    return size_;
}

template <typename T>
T& my_vector<T>::begin()
{
    return elem_[0];
}

template <typename T>
T& my_vector<T>::end()
{
    return elem_[size_];
}





#endif // MY_VECTOR_HPP
