#include <iostream>
#include <cstring>
#include <boost/array.hpp>
#include <vector>

typedef unsigned char byte_t;

template <class T, std::size_t BuffSizeV>
void serialize_bad(const T& value, boost::array<byte_t, BuffSizeV>& buffer)
{
    std::memcpy(&buffer[0], &value, sizeof(value));
}

int main()
{
    boost::array<byte_t, 4> buffer;

    serialize_bad(36274, buffer); 

    return 0;
}