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
    size_t size() const noexcept;
    T& begin();
    const T& begin() const;
    T& end();
    const T& end() const;
    void clear();

private:
    size_t size_;
    T*  elem_;
};




template <typename T>
my_vector<T>::my_vector()
    : size_{0}
    , elem_{nullptr}
{

}

template <typename T>
my_vector<T>::~my_vector()
{
    delete[] elem_;
    elem_ = nullptr;
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
    if(sz > 0)
    {
        elem_ = new T[sz];
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
    elem_ = new T(size_);
    std::copy(obj.begin(), obj.end(), elem_);
}

template <typename T>
my_vector<T>::my_vector(my_vector<T>&& obj)
{
    size_t tmp_size = size_;
    T tmp_elem[] = elem_;
    try
    {
        size_ = obj.size();
        elem_ = new T(size_);
        std::copy(obj.begin(), obj.end(), elem_);
        obj.clear();
        delete [] tmp_elem;
    }
    catch(...)
    {
        size_ = tmp_size;
        elem_ = tmp_elem;
    }
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
    elem_ = new T(size_);
    std::copy(obj.begin(), obj.end(), elem_);
    return *this;
}

template <typename T>
my_vector<T> my_vector<T>::operator=(my_vector<T>&& obj)
{
    size_t tmp_size = size_;
    T* tmp_elem = elem_;
    try
    {
        size_ = obj.size();
        elem_ = new T(size_);
        std::copy(obj.begin(), obj.end(), elem_);
        obj.clear();
        delete [] tmp_elem;
    }
    catch(...)
    {
        size_ = tmp_size;
        elem_ = tmp_elem;
    }
    return *this;
}

template <typename T>
size_t my_vector<T>::size() const noexcept
{
    return size_;
}

template <typename T>
T& my_vector<T>::begin()
{
    return elem_[0u];
}

template <typename T>
const T& my_vector<T>::begin() const
{
    return elem_[0u];
}

template <typename T>
T& my_vector<T>::end()
{
    return elem_[size_];
}

template <typename T>
const T& my_vector<T>::end() const
{
    return elem_[size_];
}

template <typename T>
void my_vector<T>::clear()
{
    size_ = 0u;
    delete [] elem_;
    elem_ = nullptr;
}




#endif // MY_VECTOR_HPP
