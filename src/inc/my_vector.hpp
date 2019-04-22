#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>


template <typename T>
class my_vector
{
    using storage_t = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
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
    size_t capacity() const noexcept;
    void clear();
    bool empty() const noexcept;
    void pop_back();
    void push_back(const T& val);
    void push_back(T&& val);
    void insert(size_t pos, T& elem);
    void insert(size_t pos, T* begin, T* end);
    void reserve(size_t capacity);
    void shrink_to_fit();
    void resize (size_t n);

    template<typename ...Args>
    void emplace(size_t pos, Args&&... elem);
    template<typename ...Args>
    void emplace_back(Args&&... args);
private:
    void copy_vec(const my_vector<T>& obj);
    void move_vec(my_vector<T>&& obj);
    void resize_storage(const size_t val);
    void release_storage(storage_t* begin, storage_t* end);
    void copy_storage(const storage_t* begin, const storage_t* end,const storage_t* destination);

    void reset_fields(const size_t& sz,const size_t& cap,const storage_t* strg, my_vector<T>& obj);

    size_t size_;
    size_t reserved_;
    storage_t*  elem_;
};


template <typename T>
my_vector<T>::my_vector()
    : size_{0u}
    , reserved_{0u}
    , elem_{nullptr}
{

}

template <typename T>
my_vector<T>::my_vector(const std::initializer_list<T>& list)
    : size_{list.size()}
    , reserved_{size_*2u}
    , elem_{new storage_t[reserved_]}
{
    storage_t* item = elem_;
    for (auto iter = list.begin(); iter != list.end(); ++iter, ++item)
    {
        new (item) T(std::forward<T>(iter));
    }
}

template <typename T>
my_vector<T>::my_vector(size_t sz)
    : size_{sz}
    , reserved_{size_*2u}
    , elem_{new storage_t[reserved_]}
{
    if(sz > 0)
    {
        new (elem_) T[size_];
    }
}

template <typename T>
my_vector<T>::my_vector(const my_vector<T>& obj)
{
    copy_vec(obj);
}

template <typename T>
my_vector<T>::my_vector(my_vector<T>&& obj)
{
    move_vec(std::forward<T>(obj));
}

template <typename T>
my_vector<T>::~my_vector()
{
    release_storage(elem_, elem_+size_);
    delete []elem_;
    elem_ = nullptr;
    size_ = 0u;
    reserved_ = 0u;
}

template <typename T>
T& my_vector<T>::operator[](size_t i)
{
    return reinterpret_cast<T&>(elem_[i]);
}

template <typename T>
const T& my_vector<T>::operator[](size_t i) const
{
    return reinterpret_cast<const T&>(elem_[i]);
}

template <typename T>
my_vector<T> my_vector<T>::operator=(const my_vector<T>& obj)
{
    copy_vec(obj);
    return *this;
}

template <typename T>
my_vector<T> my_vector<T>::operator=(my_vector<T>&& obj)
{
    move_vec(std::forward<T>(obj));
    return *this;
}

template <typename T>
T& my_vector<T>::front()
{
    return reinterpret_cast<T&>(elem_[0u]);
}

template <typename T>
T& my_vector<T>::back()
{
    return reinterpret_cast<T&>(elem_[size_-1u]);
}

template <typename T>
T& my_vector<T>::at(size_t index)
{
    if(index >= size_)
    {
        throw std::out_of_range("my_vector::at()");
    }
    return reinterpret_cast<T&>(elem_[index]);
}

template <typename T>
T& my_vector<T>::begin()
{
    return reinterpret_cast<T&>(elem_[0u]);
}

template <typename T>
T& my_vector<T>::end()
{
    return reinterpret_cast<T&>(elem_[size_]);
}

template <typename T>
const T& my_vector<T>::front() const
{
    return reinterpret_cast<const T&>(elem_[0u]);
}

template <typename T>
const T& my_vector<T>::back() const
{
    return reinterpret_cast<const T&>(elem_[size_-1]);
}

template <typename T>
const T& my_vector<T>::at(size_t index) const
{
    if(index >= size_)
    {
        throw std::out_of_range("my_vector::at()");
    }
    return reinterpret_cast<const T&>(elem_[index]);
}

template <typename T>
const T& my_vector<T>::begin() const // to do; replace with iterator
{
    return reinterpret_cast<const T&>(elem_[0u]);
}

template <typename T>
const T& my_vector<T>::end() const // to do; replace with iterator
{
    return reinterpret_cast<const T&>(elem_[size_]);
}

template <typename T>
void my_vector<T>::clear()
{
    for (storage_t* iter = elem_; iter != elem_+size_; ++iter)
    {
        reinterpret_cast<T*>(iter)->~T();
    }
    size_ = 0u;
}

template <typename T>
size_t my_vector<T>::size() const noexcept
{
    return size_;
}

template <typename T>
size_t my_vector<T>::capacity() const noexcept
{
    return reserved_;
}
template <typename T>
bool my_vector<T>::empty() const noexcept
{
    return (size_ == 0u);
}

template <typename T>
void my_vector<T>::pop_back()
{
    reinterpret_cast<T*>(&elem_[--size_])->~T();
}

template <typename T>
void my_vector<T>::push_back(const T& val)
{
    if(size_ >= reserved_)
    {
        resize_storage(2u*size_);
    }
    new (elem_+size_) T(val);
    ++size_;
}

template <typename T>
void my_vector<T>::push_back(T&& val)
{
    if(size_ >= reserved_)
    {
        resize_storage(2u*size_);
    }
    new (elem_+size_) T(std::forward<T>(val));
    ++size_;
}

template <typename T>
void my_vector<T>::insert(size_t pos, T& elem)
{
    if(pos < size_)
    {
        if(size_ == reserved_)
        {
           reserved_ = reserved_*2u;
        }

        storage_t* tmp = elem_;
        elem_ = new storage_t[reserved_];
        copy_storage(tmp, tmp + pos, elem_);
        new (elem_+pos) T(elem);
        copy_storage(tmp + pos, tmp + size_, elem_ + pos + 1);
        release_storage(tmp, tmp+size_);
        delete [] tmp;
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
           reserved_ = 2u * (size_ + insert_size) ;
        }

        storage_t* tmp = elem_;
        elem_ = new storage_t[reserved_];
        copy_storage(tmp, tmp + pos, elem_);
        copy_storage(begin, end, elem_ + pos);
        copy_storage(tmp + pos, tmp + size_, elem_ + pos + insert_size);
        size_ = new_size;
        release_storage(tmp, tmp+size_);
        delete [] tmp;
    }
    else
    {
        throw std::out_of_range("insertion pos is out of range");
    }
}

template <typename T>
template <typename ...Args>
void my_vector<T>::emplace(size_t pos, Args&&... val)
{
    if(pos < size_)
    {
        if(size_ == reserved_)
        {
           reserved_ = size_*2u;
        }

        storage_t* tmp = elem_;
        elem_ = new storage_t[reserved_];
        copy_storage(tmp, tmp + pos, elem_);
        new (elem_+pos) T(std::forward<Args>(val)...);
        copy_storage(tmp + pos, tmp + size_, elem_ + pos + 1);
        release_storage(tmp, tmp+size_);
        delete [] tmp;
        ++size_;
    }
    else
    {
        throw std::out_of_range("insertion pos is out of range");
    }
}

template <typename T >
template <typename ...Args>
void my_vector<T>::emplace_back(Args&&... val)
{
    if(size_ >= reserved_)
    {
        resize_storage(2u*size_);
    }
    new (elem_+size_) T(std::forward<Args>(val)...);
    ++size_;
}

template <typename T>
void my_vector<T>::reserve(size_t capacity)
{
    if(capacity > reserved_)
    {
        resize_storage(capacity);
    }
}

template <typename T>
void my_vector<T>::shrink_to_fit()
{
    if(size_ < reserved_)
    {
        resize_storage(size_);
    }
}

template <typename T>
void my_vector<T>::resize(size_t n)
{
    if(n < size_)
    {
        release_storage(elem_ + n, elem_+size_);
    }
    else
    {
        if(n > reserved_)
        {
            resize_storage(2u*n);
        }
        new (elem_ + size_) T[n-size_];
    }
    size_ = n;
}

template <typename T>
void my_vector<T>::copy_vec(const my_vector<T>& obj)
{
    size_ = obj.size();
    reserved_ = obj.capacity();
    elem_ = new storage_t[reserved_];
    copy_storage(obj.begin(), obj.end(), elem_);
}

template <typename T>
void my_vector<T>::move_vec(my_vector<T>&& obj)
{
    size_t tmp_size = obj.size();
    size_t tmp_reserv = obj.capacity();
    storage_t* tmp_storage = &obj[0u];
    try
    {
        size_ = obj.size();
        reserved_ = obj.capacity();
        elem_ = &obj[0u];

        reset_fields(0u, 0u, nullptr, obj);
        release_storage(tmp_storage, tmp_storage+tmp_size);
    }
    catch(...)
    {
        reset_fields(tmp_size, tmp_reserv, tmp_storage, obj);
        throw std::bad_alloc();
    }
}

template <typename T>
void my_vector<T>::resize_storage(size_t val)
{
    storage_t* tmp = elem_;
    reserved_ = val;
    elem_ = new storage_t[reserved_];
    copy_storage(tmp, tmp + size_, elem_);
    release_storage(tmp, tmp+size_);
    delete []tmp;
}

template <typename T>
void my_vector<T>::release_storage(storage_t* begin, storage_t* end)
{
    for (storage_t* iter = begin; iter != end; ++iter)
    {
        reinterpret_cast<T*>(iter)->~T();
    }
}

template <typename T>
void my_vector<T>::reset_fields(const size_t& sz,const size_t& cap,const storage_t* strg, my_vector<T>& obj )
{
    obj.size_ = sz;
    obj.reserved_= cap;
    obj.elem_ = strg;
}

template <typename T>
void my_vector<T>::copy_storage(const storage_t* begin, const storage_t* end,const storage_t* destination)
{
    for (storage_t* iter = begin; iter != end; ++iter, ++destination)
    {
        new (destination) T(std::forward<T>(reinterpret_cast<T&>(*iter)));
    }
}
#endif // MY_VECTOR_HPP
