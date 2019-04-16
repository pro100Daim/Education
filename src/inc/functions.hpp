#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "my_vector.hpp"


enum class Color {RED, YELLOW, GREEN};
enum class Lights {RED, YELLOW, GREEN};


bool float_check(std::string inp)
{
    bool isFloat = true;
    if(inp.empty())
    {
        isFloat = false;
        std::cout << "Warning: input is empty." << std::endl;
    }
    else
    {
        uint8_t dot_count = 0;
        for(std::string::iterator it = inp.begin(); it!=inp.end(); ++it)
        {
            char ch = *it;
            if(ch == '.')
            {
                ++dot_count;
            }
            if(!((ch >= '0' && ch <='9') || (ch == '.' && dot_count <= 1) || (ch == 'f' && it == inp.end() - 1 )))
            {
                isFloat = false;
            }
        }
    }
    return isFloat;
}


void read_show(int size)
{
    Float_vector vect(size);            // vector init

    for(auto i = 0; i != vect.size(); ++i)
    {
        std::string input;
        do{
            if(!input.empty())
            {
                std::cout << "Wrong input, try again."<<std::endl;
            }
            std::cin >> input;
        }
        while(!float_check(input));

        vect[i] = strtof(input.c_str(), 0);                 // read into storage
    }

    for(auto j = 0; j != vect.size(); ++j)
    {
        std::cout << vect[j] << std::endl;
    }
}

#endif // FUNCTIONS_HPP
