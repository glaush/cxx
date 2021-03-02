#include <iostream>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>
#include <cassert>


class negative_number
{
    unsigned short number_;

public:
    
    explicit negative_number(unsigned short number = 0)
        : number_(number)
    {

    }

    unsigned short value_without_sign() const
    {
        return number_;
    }

};

inline std::ostream& operator<<(std::ostream& os, const negative_number& num)
{
    return os << "-" << num.value_without_sign();
}

inline std::istream& operator>>(std::istream& is, negative_number& num)
{
    char ch;
    is >> ch;

    if (ch != '-')
    {
        throw std::logic_error("Negative number class stores only negative values.");
    }

    unsigned short s;
    is >> s;
    num = negative_number(s);

    return is;
}


void main()
{
    const negative_number n = boost::lexical_cast<negative_number>("-100");

    assert(n.value_without_sign() == 100);

    const int i = boost::lexical_cast<int>(n);

    assert(i == -100);

    typedef boost::array<char, 10> arr_t;

    const arr_t arr = boost::lexical_cast<arr_t>(n);

    assert(arr[0] == '-');
    assert(arr[1] == '1');
    assert(arr[2] == '0');
    assert(arr[3] == '0');

    //thow an exception

    assert(arr[4] == '0');

    return;
}