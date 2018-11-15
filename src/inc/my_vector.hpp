#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <iostream>

class Float_vector
{
public:
    Float_vector() : sz{0}{}
    Float_vector(int size) : sz{size}, elem{new float[size]} {}
    ~Float_vector()
    {
        delete elem;
    }

    float& operator[](int i)
    {
        return elem[i];
    }

    int size()
    {
        return sz;
    }

private:
    int sz;
    float *elem;
};

enum class Color {RED, YELLOW, GREEN};
enum class Lights {RED, YELLOW, GREEN};


#endif // MY_VECTOR_HPP
